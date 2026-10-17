/* WebSocket over WinHTTP (ws_winhttp.h). WinHTTP in synchronous mode allows one send and one
   receive at a time on a WebSocket, from different threads, so each has its own thread;
   closing the handle cancels a pending receive. */
#include "ws_winhttp.h"

#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Out { struct Out *next; int binary; size_t n; uint8_t data[]; } Out;

struct WsConn {
    WsHandler h;
    char url[512];
    HINTERNET session, connection;
    HINTERNET socket;            /* the open WebSocket, NULL between connections */
    CRITICAL_SECTION lock;       /* socket and the queue */
    CRITICAL_SECTION send_lock;  /* one send at a time */
    Out *head, *tail;
    HANDLE wake, stop_event, io_thread, send_thread;
    volatile LONG stopping;
};

/* Opens the WebSocket; on failure returns NULL with why in error. */
static HINTERNET ws_connect(WsConn *c, const char *url, char *error, size_t cap)
{
    wchar_t wurl[600], host[256], path[512];
    URL_COMPONENTS uc;
    HINTERNET req = NULL, ws = NULL;
    DWORD status = 0, size = sizeof status;
    char http[600];
    snprintf(http, sizeof http, "%s%s", !strncmp(url, "wss://", 6) ? "https://" : !strncmp(url, "ws://", 5) ? "http://" : "",
             !strncmp(url, "wss://", 6) ? url + 6 : !strncmp(url, "ws://", 5) ? url + 5 : url);
    MultiByteToWideChar(CP_UTF8, 0, http, -1, wurl, 600);
    memset(&uc, 0, sizeof uc);
    uc.dwStructSize = sizeof uc;
    uc.lpszHostName = host, uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path, uc.dwUrlPathLength = 512;
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        snprintf(error, cap, "bad URL %s", url);
        return NULL;
    }
    c->connection = WinHttpConnect(c->session, host, uc.nPort, 0);
    if (c->connection)
        req = WinHttpOpenRequest(c->connection, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                 uc.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (req && WinHttpSetOption(req, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL, 0) &&
        WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(req, NULL) &&
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                            &status, &size, WINHTTP_NO_HEADER_INDEX)) {
        if (status == 101)
            ws = WinHttpWebSocketCompleteUpgrade(req, 0);
        else
            snprintf(error, cap, "HTTP %lu from %s", status, url);
    } else
        snprintf(error, cap, "can't connect to %s (error %lu)", url, GetLastError());
    if (req)
        WinHttpCloseHandle(req);
    if (!ws && c->connection)
        WinHttpCloseHandle(c->connection), c->connection = NULL;
    if (ws) {
        DWORD never = 0;
        WinHttpSetOption(ws, WINHTTP_OPTION_RECEIVE_TIMEOUT, &never, sizeof never);
    }
    return ws;
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
        HINTERNET ws = ws_connect(c, c->url, reason, sizeof reason);
        if (ws) {
            size_t len = 0;
            attempt = 0;
            EnterCriticalSection(&c->lock);
            clear_queue(c);
            c->socket = ws;
            LeaveCriticalSection(&c->lock);
            c->h.on_open(c->h.ctx, c);
            for (;;) {
                DWORD got = 0;
                WINHTTP_WEB_SOCKET_BUFFER_TYPE type;
                DWORD r = WinHttpWebSocketReceive(ws, buf + len, (DWORD)(cap - len), &got, &type);
                if (r != NO_ERROR) {
                    code = 1006;
                    snprintf(reason, sizeof reason, "connection lost (error %lu)", r);
                    break;
                }
                if (type == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE) {
                    USHORT status = 1005;
                    DWORD n = 0;
                    WinHttpWebSocketQueryCloseStatus(ws, &status, reason, sizeof reason - 1, &n);
                    reason[n] = 0;
                    code = status;
                    EnterCriticalSection(&c->send_lock);
                    if (c->socket == ws)
                        WinHttpWebSocketClose(ws, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, NULL, 0);
                    LeaveCriticalSection(&c->send_lock);
                    break;
                }
                len += got;
                if (type == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE ||
                    type == WINHTTP_WEB_SOCKET_BINARY_FRAGMENT_BUFFER_TYPE) {
                    if (len == cap)
                        buf = realloc(buf, (cap *= 2) + 1);
                    continue;
                }
                buf[len] = 0;
                c->h.on_message(c->h.ctx, type == WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE, buf, len);
                len = 0;
            }
            EnterCriticalSection(&c->send_lock);
            EnterCriticalSection(&c->lock);
            if (c->socket == ws)  /* (else ws_stop or a tick closed it already) */
                WinHttpCloseHandle(ws);
            c->socket = NULL;
            LeaveCriticalSection(&c->lock);
            LeaveCriticalSection(&c->send_lock);
            WinHttpCloseHandle(c->connection);
            c->connection = NULL;
        }
        next = c->h.on_close(c->h.ctx, code, reason, attempt++);
        if (!next || c->stopping)
            break;
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
    if (c->socket)
        WinHttpCloseHandle(c->socket), c->socket = NULL;
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
            EnterCriticalSection(&c->send_lock);
            EnterCriticalSection(&c->lock);
            if ((m = c->head) && !(c->head = m->next))
                c->tail = NULL;
            LeaveCriticalSection(&c->lock);
            if (m && c->socket)
                WinHttpWebSocketSend(c->socket, m->binary ? WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE
                                                          : WINHTTP_WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE, m->data, (DWORD)m->n);
            LeaveCriticalSection(&c->send_lock);
            if (!m)
                break;
            free(m);
        }
        if (c->h.on_tick && c->h.on_tick(c->h.ctx))
            force_close(c);
    }
    return 0;
}

WsConn *ws_start(const char *url, const WsHandler *h)
{
    WsConn *c = calloc(1, sizeof *c);
    c->h = *h;
    snprintf(c->url, sizeof c->url, "%s", url);
    c->session = WinHttpOpen(L"dhd-port", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME,
                             WINHTTP_NO_PROXY_BYPASS, 0);
    if (!c->session)  /* before Windows 8.1 */
        c->session = WinHttpOpen(L"dhd-port", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME,
                                 WINHTTP_NO_PROXY_BYPASS, 0);
    WinHttpSetTimeouts(c->session, 5000, 5000, 5000, 10000);
    InitializeCriticalSection(&c->lock);
    InitializeCriticalSection(&c->send_lock);
    c->wake = CreateEventW(NULL, FALSE, FALSE, NULL);
    c->stop_event = CreateEventW(NULL, TRUE, FALSE, NULL);
    c->io_thread = CreateThread(NULL, 0, io_main, c, 0, NULL);
    c->send_thread = CreateThread(NULL, 0, send_main, c, 0, NULL);
    return c;
}

void ws_send(WsConn *c, int binary, const void *data, size_t n)
{
    Out *m;
    EnterCriticalSection(&c->lock);
    if (!c->socket) {
        LeaveCriticalSection(&c->lock);
        return;
    }
    m = malloc(sizeof *m + n);
    m->next = NULL, m->binary = binary, m->n = n;
    memcpy(m->data, data, n);
    if (c->tail)
        c->tail->next = m;
    else
        c->head = m;
    c->tail = m;
    LeaveCriticalSection(&c->lock);
    SetEvent(c->wake);
}

void ws_stop(WsConn *c)
{
    int i;
    for (i = 0; i < 50 && c->head; i++)  /* let the queue drain (a leave message) */
        Sleep(10);
    InterlockedExchange(&c->stopping, 1);
    SetEvent(c->stop_event);
    SetEvent(c->wake);
    WaitForSingleObject(c->send_thread, INFINITE);
    EnterCriticalSection(&c->send_lock);
    if (c->socket)
        WinHttpWebSocketShutdown(c->socket, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, NULL, 0);
    LeaveCriticalSection(&c->send_lock);
    if (WaitForSingleObject(c->io_thread, 2000) == WAIT_TIMEOUT) {
        force_close(c);
        WaitForSingleObject(c->io_thread, INFINITE);
    }
    CloseHandle(c->io_thread), CloseHandle(c->send_thread);
    CloseHandle(c->wake), CloseHandle(c->stop_event);
    clear_queue(c);
    WinHttpCloseHandle(c->session);
    DeleteCriticalSection(&c->lock), DeleteCriticalSection(&c->send_lock);
    free(c);
}

int http_get(const char *url, char *out, size_t cap)
{
    wchar_t wurl[600], host[256], path[512];
    char http[600];
    URL_COMPONENTS uc;
    HINTERNET session, conn = NULL, req = NULL;
    DWORD status = 0, size = sizeof status, got;
    int len = -1;
    snprintf(http, sizeof http, "%s%s", !strncmp(url, "wss://", 6) ? "https://" : !strncmp(url, "ws://", 5) ? "http://" : "",
             !strncmp(url, "wss://", 6) ? url + 6 : !strncmp(url, "ws://", 5) ? url + 5 : url);
    MultiByteToWideChar(CP_UTF8, 0, http, -1, wurl, 600);
    memset(&uc, 0, sizeof uc);
    uc.dwStructSize = sizeof uc;
    uc.lpszHostName = host, uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path, uc.dwUrlPathLength = 512;
    if (!cap || !WinHttpCrackUrl(wurl, 0, 0, &uc))
        return -1;
    session = WinHttpOpen(L"dhd-port", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (session) {
        WinHttpSetTimeouts(session, 5000, 5000, 5000, 10000);
        conn = WinHttpConnect(session, host, uc.nPort, 0);
    }
    if (conn)
        req = WinHttpOpenRequest(conn, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                 uc.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (req && WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(req, NULL) &&
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                            &status, &size, WINHTTP_NO_HEADER_INDEX) && status == 200) {
        len = 0;
        while ((size_t)len + 1 < cap && WinHttpReadData(req, out + len, (DWORD)(cap - 1 - len), &got) && got)
            len += (int)got;
        out[len] = 0;
    }
    if (req)
        WinHttpCloseHandle(req);
    if (conn)
        WinHttpCloseHandle(conn);
    if (session)
        WinHttpCloseHandle(session);
    return len;
}
