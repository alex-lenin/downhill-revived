/* A WebSocket client on WinHTTP (built into Windows 8 and later): a receive thread that
   connects, reads messages and reconnects when told to, and a send thread that drains a queue
   and ticks the handler every 10 ms. */
#ifndef NET_WS_WINHTTP_H
#define NET_WS_WINHTTP_H

#include <stddef.h>
#include <stdint.h>

typedef struct WsConn WsConn;

typedef struct {
    void *ctx;
    void (*on_open)(void *ctx, WsConn *ws);
    void (*on_message)(void *ctx, int binary, const uint8_t *data, size_t n);  /* data[n] is 0 */
    /* The connection ended or failed (code -1: connecting failed). Returns the URL to connect
       to next, after a backoff, or NULL to stop. attempt counts failures since the last open. */
    const char *(*on_close)(void *ctx, int code, const char *reason, int attempt);
    int (*on_tick)(void *ctx);  /* 1: drop the connection (the server went silent) */
} WsHandler;

WsConn *ws_start(const char *url, const WsHandler *h);
/* Queues a message; dropped when not connected. */
void ws_send(WsConn *ws, int binary, const void *data, size_t n);
/* Sends what is queued, closes, stops the threads and frees ws. */
void ws_stop(WsConn *ws);

/* A blocking HTTP(S) GET; ws:// and wss:// URLs work too. Returns the body length, -1 on error. */
int http_get(const char *url, char *out, size_t cap);

#endif
