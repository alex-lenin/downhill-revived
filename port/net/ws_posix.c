/* A WebSocket client on POSIX sockets (ws_winhttp.h): same reconnect, queue,
   tick and close semantics as ws_winhttp.c. Plain ws:// and http:// only;
   wss:// and https:// fail with a message (no TLS stack on this host). Only
   the boot's dormancy matters here (no online races on headless Linux yet);
   the code paths below are the real client, not placeholders. */
#include "ws_winhttp.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../host/win32_compat.h"

typedef struct Out { struct Out *next; int binary; size_t n; uint8_t data[]; } Out;

struct WsConn {
    WsHandler h;
    char url[512];
    int sock; /* the open socket, -1 between connections */
    CRITICAL_SECTION lock;      /* sock and the queue */
    CRITICAL_SECTION send_lock; /* one send at a time */
    Out *head, *tail;
    HANDLE wake, stop_event, io_thread, send_thread;
    volatile LONG stopping;
};

/* ---- small crypto: SHA-1 (RFC 3174) and base64, for the ws handshake ---- */

typedef struct {
    uint32_t h[5];
    uint64_t n;
    uint8_t block[64];
    size_t used;
} Sha1;

static uint32_t sha1_rol(uint32_t v, int n) { return (v << n) | (v >> (32 - n)); }

static void sha1_block(Sha1 *s, const uint8_t *p)
{
    uint32_t w[80];
    for (int i = 0; i < 16; i++) w[i] = (uint32_t)p[4 * i] << 24 | (uint32_t)p[4 * i + 1] << 16 |
                                        (uint32_t)p[4 * i + 2] << 8 | p[4 * i + 3];
    for (int i = 16; i < 80; i++) w[i] = sha1_rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    uint32_t a = s->h[0], b = s->h[1], c = s->h[2], d = s->h[3], e = s->h[4];
    for (int i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) f = (b & c) | (~b & d), k = 0x5A827999;
        else if (i < 40) f = b ^ c ^ d, k = 0x6ED9EBA1;
        else if (i < 60) f = (b & c) | (b & d) | (c & d), k = 0x8F1BBCDC;
        else f = b ^ c ^ d, k = 0xCA62C1D6;
        uint32_t t = sha1_rol(a, 5) + f + e + k + w[i];
        e = d, d = c, c = sha1_rol(b, 30), b = a, a = t;
    }
    s->h[0] += a, s->h[1] += b, s->h[2] += c, s->h[3] += d, s->h[4] += e;
}

static void sha1(const uint8_t *data, size_t n, uint8_t out[20])
{
    Sha1 s = {{0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0}, 0, {0}, 0};
    while (n) {
        size_t take = 64 - s.used;
        if (take > n) take = n;
        memcpy(s.block + s.used, data, take);
        s.used += take, data += take, n -= take, s.n += take;
        if (s.used == 64) sha1_block(&s, s.block), s.used = 0;
    }
    uint64_t bits = s.n * 8;
    uint8_t pad = 0x80;
    size_t extra = s.used < 56 ? 56 - s.used : 120 - s.used;
    /* Feed padding through the same path (block at a time). */
    const uint8_t *p = &pad;
    size_t left = 1 + extra - 1;
    while (left) {
        size_t take = 64 - s.used;
        if (take > left) take = left;
        if (p == &pad && take) { memcpy(s.block + s.used, &pad, 1); p = NULL; take--; s.used++, s.n++, left--; }
        else { memset(s.block + s.used, 0, take); s.used += take, s.n += take, left -= take; }
        if (s.used == 64) sha1_block(&s, s.block), s.used = 0;
    }
    for (int i = 7; i >= 0; i--) {
        if (s.used == 64) sha1_block(&s, s.block), s.used = 0;
        s.block[s.used++] = (uint8_t)(bits >> (8 * i)), s.n++;
    }
    sha1_block(&s, s.block);
    for (int i = 0; i < 5; i++) {
        out[4 * i] = (uint8_t)(s.h[i] >> 24), out[4 * i + 1] = (uint8_t)(s.h[i] >> 16);
        out[4 * i + 2] = (uint8_t)(s.h[i] >> 8), out[4 * i + 3] = (uint8_t)s.h[i];
    }
}

static void base64(const uint8_t *in, size_t n, char *out)
{
    static const char digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    while (n >= 3) {
        *out++ = digits[in[0] >> 2], *out++ = digits[((in[0] & 3) << 4) | (in[1] >> 4)];
        *out++ = digits[((in[1] & 15) << 2) | (in[2] >> 6)], *out++ = digits[in[2] & 63];
        in += 3, n -= 3;
    }
    if (n == 1) {
        *out++ = digits[in[0] >> 2], *out++ = digits[(in[0] & 3) << 4], *out++ = '=', *out++ = '=';
    } else if (n == 2) {
        *out++ = digits[in[0] >> 2], *out++ = digits[((in[0] & 3) << 4) | (in[1] >> 4)];
        *out++ = digits[(in[1] & 15) << 2], *out++ = '=';
    }
    *out = 0;
}

/* ---- URLs and TCP ---- */

/* ws:// or http:// URL into host, port and path; 0 with why for anything else. */
static int split_url(const char *url, int websockets, char *host, size_t host_cap, int *port, char *path,
                     size_t path_cap, char *why, size_t why_cap)
{
    const char *prefix = websockets ? "ws://" : "http://";
    const char *slash, *colon;
    if (!strncmp(url, "wss://", 6) || !strncmp(url, "https://", 8)) {
        snprintf(why, why_cap, "no TLS on this host: %s", url);
        return 0;
    }
    if (strncmp(url, prefix, strlen(prefix)) && (!websockets || strncmp(url, "http://", 7))) {
        /* http_get also takes ws:// URLs (plain HTTP on their port). */
        if (!websockets && !strncmp(url, "ws://", 5)) url += 5;
        else {
            snprintf(why, why_cap, "bad URL %s", url);
            return 0;
        }
    } else {
        url += strchr(url, ':')[1] == '/' ? strlen(prefix) : 7;
    }
    slash = strchr(url, '/');
    colon = strchr(url, ':');
    if (colon && (!slash || colon < slash)) {
        size_t n = (size_t)(colon - url);
        if (n >= host_cap) n = host_cap - 1;
        memcpy(host, url, n), host[n] = 0;
        *port = atoi(colon + 1);
    } else {
        size_t n = slash ? (size_t)(slash - url) : strlen(url);
        if (n >= host_cap) n = host_cap - 1;
        memcpy(host, url, n), host[n] = 0;
        *port = 80;
    }
    snprintf(path, path_cap, "%s", slash ? slash : "/");
    return *port > 0 && *port < 65536;
}

/* A connected TCP socket, or -1 (5 s to connect, 10 s to receive). */
static int tcp_connect(const char *host, int port)
{
    struct addrinfo hints = {0}, *list = NULL, *a;
    char service[16];
    int sock = -1;
    hints.ai_family = AF_UNSPEC, hints.ai_socktype = SOCK_STREAM;
    snprintf(service, sizeof service, "%d", port);
    if (getaddrinfo(host, service, &hints, &list) != 0) return -1;
    for (a = list; a; a = a->ai_next) {
        int flags, r;
        sock = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
        if (sock < 0) continue;
        flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
        r = connect(sock, a->ai_addr, a->ai_addrlen);
        if (r < 0 && errno != EINPROGRESS) {
            close(sock), sock = -1;
            continue;
        }
        if (r < 0) {
            fd_set w;
            struct timeval t = {5, 0};
            FD_ZERO(&w);
        FD_SET(sock, &w);
            if (select(sock + 1, NULL, &w, NULL, &t) <= 0) {
                close(sock), sock = -1;
                continue;
            }
            int err = 0;
            socklen_t n = sizeof err;
            if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &n) < 0 || err) {
                close(sock), sock = -1;
                continue;
            }
        }
        fcntl(sock, F_SETFL, flags);
        break;
    }
    freeaddrinfo(list);
    if (sock >= 0) {
        struct timeval t = {10, 0};
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof t);
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof t);
    }
    return sock;
}

static int send_all(int sock, const void *data, size_t n)
{
    while (n) {
        ssize_t r = send(sock, data, n, MSG_NOSIGNAL);
        if (r <= 0) return 0;
        data = (const uint8_t *)data + r, n -= r;
    }
    return 1;
}

/* Until end-of-headers (\r\n\r\n): the header block, NUL-terminated, or NULL. */
static char *read_headers(int sock, char *buf, size_t cap, size_t *used)
{
    size_t n = 0;
    for (;;) {
        ssize_t r;
        if (n + 1 >= cap) return NULL;
        r = recv(sock, buf + n, cap - 1 - n, 0);
        if (r <= 0) return NULL;
        n += r, buf[n] = 0;
        if (strstr(buf, "\r\n\r\n")) {
            *used = n;
            return buf;
        }
    }
}

static int header_status(const char *headers)
{
    int status = 0;
    sscanf(headers, "HTTP/%*d.%*d %d", &status);
    return status;
}

/* The header's value (into out), or NULL. */
static const char *header_value(const char *headers, const char *name, char *out, size_t cap)
{
    size_t n = strlen(name);
    const char *p = headers;
    for (;;) {
        const char *eol;
        p = strstr(p, "\r\n");
        if (!p) return NULL;
        p += 2;
        if (!strncasecmp(p, name, n) && p[n] == ':') {
            p += n + 1;
            while (*p == ' ') p++;
            eol = strstr(p, "\r\n");
            if (!eol || (size_t)(eol - p) >= cap) return NULL;
            memcpy(out, p, eol - p), out[eol - p] = 0;
            return out;
        }
    }
}

/* ---- the WebSocket wire ---- */

/* Opens the WebSocket; on failure returns -1 with why in error. */
static int ws_connect(WsConn *c, const char *url, char *error, size_t cap)
{
    char host[256], path[512], key[32], accept[32], check[64], headers[4096];
    uint8_t raw[16], digest[20], line[512];
    int port, sock;
    size_t used = 0;
    FILE *random;
    if (!split_url(url, 1, host, sizeof host, &port, path, sizeof path, error, cap)) return -1;
    random = fopen("/dev/urandom", "rb");
    if (!random || fread(raw, 1, sizeof raw, random) != sizeof raw) {
        if (random) fclose(random);
        snprintf(error, cap, "can't make a ws key");
        return -1;
    }
    fclose(random);
    base64(raw, sizeof raw, key);
    sock = tcp_connect(host, port);
    if (sock < 0) {
        snprintf(error, cap, "can't connect to %s", url);
        return -1;
    }
    snprintf((char *)line, sizeof line,
             "GET %s HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n"
             "Sec-WebSocket-Key: %s\r\nSec-WebSocket-Version: 13\r\n\r\n",
             path, host, port, key);
    if (!send_all(sock, line, strlen((char *)line)) || !read_headers(sock, headers, sizeof headers, &used)) {
        snprintf(error, cap, "can't connect to %s", url);
        close(sock);
        return -1;
    }
    if (header_status(headers) != 101 ||
        !header_value(headers, "Sec-WebSocket-Accept", check, sizeof check)) {
        snprintf(error, cap, "HTTP %d from %s", header_status(headers), url);
        close(sock);
        return -1;
    }
    snprintf((char *)line, sizeof line, "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", key);
    sha1(line, strlen((char *)line), digest);
    base64(digest, sizeof digest, accept);
    if (strcmp(accept, check)) {
        snprintf(error, cap, "bad ws accept from %s", url);
        close(sock);
        return -1;
    }
    return sock;
}

/* One frame header; the payload follows (mask applied by the caller). */
static int ws_frame(int sock, int *opcode, size_t *length, uint8_t mask[4])
{
    uint8_t h[2];
    size_t n = 0;
    while (n < 2) {
        ssize_t r = recv(sock, h + n, 2 - n, 0);
        if (r <= 0) return 0;
        n += r;
    }
    *opcode = h[0] & 15;
    *length = h[1] & 127;
    if (h[1] & 128) {
        n = 0;
        while (n < 4) {
            ssize_t r = recv(sock, mask + n, 4 - n, 0);
            if (r <= 0) return 0;
            n += r;
        }
    } else {
        memset(mask, 0, 4);
    }
    if (*length == 126) {
        uint8_t e[2];
        n = 0;
        while (n < 2) {
            ssize_t r = recv(sock, e + n, 2 - n, 0);
            if (r <= 0) return 0;
            n += r;
        }
        *length = (size_t)e[0] << 8 | e[1];
    } else if (*length == 127) {
        uint8_t e[8];
        uint64_t v = 0;
        n = 0;
        while (n < 8) {
            ssize_t r = recv(sock, e + n, 8 - n, 0);
            if (r <= 0) return 0;
            n += r;
        }
        for (int i = 0; i < 8; i++) v = v << 8 | e[i];
        if (v > 64 * 1024 * 1024) return 0;
        *length = (size_t)v;
    }
    return 1;
}

static int ws_payload(int sock, uint8_t *buf, size_t n, const uint8_t mask[4])
{
    size_t got = 0, off = 0;
    while (got < n) {
        ssize_t r = recv(sock, buf + got, n - got, 0);
        if (r <= 0) return 0;
        got += r;
    }
    for (size_t i = 0; i < n; i++) buf[i] ^= mask[(off + i) & 3];
    return 1;
}

/* A masked client frame. */
static int ws_send_frame(int sock, int opcode, const void *data, size_t n)
{
    uint8_t h[14], mask[4], *p = h + 2;
    FILE *random = fopen("/dev/urandom", "rb");
    size_t head;
    if (!random || fread(mask, 1, sizeof mask, random) != sizeof mask) {
        if (random) fclose(random);
        return 0;
    }
    fclose(random);
    h[0] = (uint8_t)(0x80 | opcode);
    if (n < 126) h[1] = (uint8_t)(0x80 | n);
    else if (n < 65536) h[1] = 0x80 | 126, *p++ = (uint8_t)(n >> 8), *p++ = (uint8_t)n;
    else h[1] = 0x80 | 127, *p++ = 0, *p++ = 0, *p++ = 0, *p++ = 0,
         *p++ = (uint8_t)(n >> 24), *p++ = (uint8_t)(n >> 16), *p++ = (uint8_t)(n >> 8), *p++ = (uint8_t)n;
    memcpy(p, mask, 4), p += 4;
    head = p - h;
    if (!send_all(sock, h, head)) return 0;
    /* Mask on the way out (a chunk at a time, so big race frames stay small). */
    const uint8_t *q = data;
    uint8_t chunk[4096];
    for (size_t off = 0; off < n;) {
        size_t take = n - off > sizeof chunk ? sizeof chunk : n - off;
        for (size_t i = 0; i < take; i++) chunk[i] = q[off + i] ^ mask[(off + i) & 3];
        if (!send_all(sock, chunk, take)) return 0;
        off += take;
    }
    return 1;
}

static void clear_queue(WsConn *c)
{
    Out *m;
    while ((m = c->head))
        c->head = m->next, free(m);
    c->tail = NULL;
}

static DWORD WINAPI io_main(void *arg)
{
    WsConn *c = arg;
    int attempt = 0;
    size_t cap = 4096;
    uint8_t *buf = malloc(cap + 1);
    while (!c->stopping) {
        char reason[160] = "";
        int code = -1;
        const char *next;
        int sock = ws_connect(c, c->url, reason, sizeof reason);
        if (sock >= 0) {
            size_t len = 0;
            int opcode = -1;
            attempt = 0;
            EnterCriticalSection(&c->lock);
            clear_queue(c);
            c->sock = sock;
            LeaveCriticalSection(&c->lock);
            c->h.on_open(c->h.ctx, c);
            for (;;) {
                int frame;
                size_t length;
                uint8_t mask[4];
                if (!ws_frame(sock, &frame, &length, mask) ||
                    ((frame == 8 || frame == 9 || frame == 10) && length > 125)) {
                    code = 1006;
                    snprintf(reason, sizeof reason, "connection lost");
                    break;
                }
                if (frame == 8) {
                    uint8_t close_body[128];
                    size_t n = length > sizeof close_body ? sizeof close_body : length;
                    code = 1005;
                    if (n >= 2 && ws_payload(sock, close_body, n, mask)) {
                        code = close_body[0] << 8 | close_body[1];
                        size_t r = n - 2 < sizeof reason - 1 ? n - 2 : sizeof reason - 1;
                        memcpy(reason, close_body + 2, r), reason[r] = 0;
                    }
                    EnterCriticalSection(&c->send_lock);
                    EnterCriticalSection(&c->lock);
                    if (c->sock == sock) ws_send_frame(sock, 8, close_body, n >= 2 ? 2 : 0);
                    LeaveCriticalSection(&c->lock);
                    LeaveCriticalSection(&c->send_lock);
                    break;
                }
                if (frame == 9) {
                    uint8_t ping[128];
                    size_t n = length > sizeof ping ? sizeof ping : length;
                    if (ws_payload(sock, ping, n, mask)) {
                        EnterCriticalSection(&c->send_lock);
                        EnterCriticalSection(&c->lock);
                        if (c->sock == sock) ws_send_frame(sock, 10, ping, n);
                        LeaveCriticalSection(&c->lock);
                        LeaveCriticalSection(&c->send_lock);
                    }
                    continue;
                }
                if (frame == 10) {
                    uint8_t pong[128];
                    size_t n = length > sizeof pong ? sizeof pong : length;
                    if (!ws_payload(sock, pong, n, mask)) {
                        code = 1006;
                        snprintf(reason, sizeof reason, "connection lost");
                        break;
                    }
                    continue;
                }
                if (frame != 0 && frame != 1 && frame != 2) {
                    code = 1006;
                    snprintf(reason, sizeof reason, "bad frame");
                    break;
                }
                if (frame != 0) {
                    opcode = frame;
                    len = 0;
                }
                if (opcode < 0 || len + length > 64 * 1024 * 1024) {
                    code = 1006;
                    snprintf(reason, sizeof reason, "bad fragment");
                    break;
                }
                if (len + length + 1 > cap) {
                    while (len + length + 1 > cap) cap *= 2;
                    buf = realloc(buf, cap + 1);
                }
                if (!ws_payload(sock, buf + len, length, mask)) {
                    code = 1006;
                    snprintf(reason, sizeof reason, "connection lost");
                    break;
                }
                len += length;
                if (frame == 0) continue; /* more fragments */
                buf[len] = 0;
                c->h.on_message(c->h.ctx, opcode == 2, buf, len);
                len = 0;
            }
            EnterCriticalSection(&c->send_lock);
            EnterCriticalSection(&c->lock);
            if (c->sock == sock) { /* (else ws_stop or a tick closed it already) */
                shutdown(sock, SHUT_RDWR);
                close(sock);
            }
            c->sock = -1;
            LeaveCriticalSection(&c->lock);
            LeaveCriticalSection(&c->send_lock);
        }
        next = c->h.on_close(c->h.ctx, code, reason, attempt++);
        if (!next || c->stopping) break;
        snprintf(c->url, sizeof c->url, "%s", next);
        WaitForSingleObject(c->stop_event, attempt < 4 ? 500u << attempt : 8000);
    }
    free(buf);
    return 0;
}

/* Drops the connection from outside the receive thread: its pending receive fails. */
static void force_close(WsConn *c)
{
    EnterCriticalSection(&c->send_lock);
    EnterCriticalSection(&c->lock);
    if (c->sock >= 0) {
        shutdown(c->sock, SHUT_RDWR);
        close(c->sock);
        c->sock = -1;
    }
    LeaveCriticalSection(&c->lock);
    LeaveCriticalSection(&c->send_lock);
}

static DWORD WINAPI send_main(void *arg)
{
    WsConn *c = arg;
    while (!c->stopping) {
        Out *m;
        WaitForSingleObject(c->wake, 10);
        for (;;) {
            int sock;
            EnterCriticalSection(&c->send_lock);
            EnterCriticalSection(&c->lock);
            if ((m = c->head) && !(c->head = m->next)) c->tail = NULL;
            sock = c->sock;
            LeaveCriticalSection(&c->lock);
            if (m && sock >= 0) ws_send_frame(sock, m->binary ? 2 : 1, m->data, m->n);
            LeaveCriticalSection(&c->send_lock);
            if (!m) break;
            free(m);
        }
        if (c->h.on_tick && c->h.on_tick(c->h.ctx)) force_close(c);
    }
    return 0;
}

WsConn *ws_start(const char *url, const WsHandler *h)
{
    WsConn *c = calloc(1, sizeof *c);
    c->h = *h;
    snprintf(c->url, sizeof c->url, "%s", url);
    c->sock = -1;
    InitializeCriticalSection(&c->lock);
    InitializeCriticalSection(&c->send_lock);
    c->wake = CreateEventA(NULL, FALSE, FALSE, NULL);
    c->stop_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    c->io_thread = CreateThread(NULL, 0, io_main, c, 0, NULL);
    c->send_thread = CreateThread(NULL, 0, send_main, c, 0, NULL);
    return c;
}

void ws_send(WsConn *c, int binary, const void *data, size_t n)
{
    Out *m;
    EnterCriticalSection(&c->lock);
    if (c->sock < 0) {
        LeaveCriticalSection(&c->lock);
        return;
    }
    m = malloc(sizeof *m + n);
    m->next = NULL, m->binary = binary, m->n = n;
    memcpy(m->data, data, n);
    if (c->tail) c->tail->next = m;
    else c->head = m;
    c->tail = m;
    LeaveCriticalSection(&c->lock);
    SetEvent(c->wake);
}

void ws_stop(WsConn *c)
{
    int i;
    for (i = 0; i < 50 && c->head; i++) /* let the queue drain (a leave message) */
        Sleep(10);
    InterlockedExchange(&c->stopping, 1);
    SetEvent(c->stop_event);
    SetEvent(c->wake);
    WaitForSingleObject(c->send_thread, INFINITE);
    EnterCriticalSection(&c->send_lock);
    if (c->sock >= 0) {
        ws_send_frame(c->sock, 8, "\x03\xe8", 2);
        shutdown(c->sock, SHUT_RDWR);
        close(c->sock);
        c->sock = -1;
    }
    LeaveCriticalSection(&c->send_lock);
    if (WaitForSingleObject(c->io_thread, 2000) == WAIT_TIMEOUT) {
        force_close(c);
        WaitForSingleObject(c->io_thread, INFINITE);
    }
    CloseHandle(c->io_thread), CloseHandle(c->send_thread);
    CloseHandle(c->wake), CloseHandle(c->stop_event);
    clear_queue(c);
    DeleteCriticalSection(&c->lock), DeleteCriticalSection(&c->send_lock);
    free(c);
}

int http_get(const char *url, char *out, size_t cap)
{
    char host[256], path[512], headers[4096], http[600];
    const char *body;
    int port, sock;
    size_t used = 0, total = 0;
    int len = -1;
    snprintf(http, sizeof http, "%s%s", !strncmp(url, "wss://", 6) ? "https://" : !strncmp(url, "ws://", 5) ? "http://" : "",
             !strncmp(url, "wss://", 6) ? url + 6 : !strncmp(url, "ws://", 5) ? url + 5 : url);
    if (!cap || !split_url(http, 0, host, sizeof host, &port, path, sizeof path, out, cap)) return -1;
    sock = tcp_connect(host, port);
    if (sock < 0) return -1;
    snprintf(headers, sizeof headers, "GET %s HTTP/1.0\r\nHost: %s:%d\r\nConnection: close\r\n\r\n", path, host, port);
    if (send_all(sock, headers, strlen(headers)) && read_headers(sock, headers, sizeof headers, &used) &&
        header_status(headers) == 200) {
        len = 0;
        body = strstr(headers, "\r\n\r\n");
        body = body ? body + 4 : headers + used;
        total = used - (body - headers);
        if (total > cap - 1) total = cap - 1;
        memcpy(out, body, total);
        len = (int)total;
        while ((size_t)len + 1 < cap) {
            ssize_t r = recv(sock, out + len, cap - 1 - len, 0);
            if (r <= 0) break;
            len += (int)r;
        }
        out[len] = 0;
    }
    close(sock);
    return len;
}
