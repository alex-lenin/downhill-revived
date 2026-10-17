/* net.h's API: one client (client.c) over one WebSocket (ws_winhttp.c). The relay's address
   comes from net_init or the DHD_NET_SERVER environment variable (the official server unless
   dhd.ini or the environment names another, port/hle/host.c). */
#include "net.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "json.h"
#include "ws_winhttp.h"

enum { REJOIN_ATTEMPTS = 5 };

static NetClient client;
static WsConn *conn;
static int inited;
static char base[256], name[100], version[64], iso[65], next_url[300];
static CRITICAL_SECTION api;  /* serialises connect, leave and the reset between rooms */

static void send_text(void *ws, const char *text) { ws_send(ws, 0, text, strlen(text)); }
static void send_binary(void *ws, const void *data, size_t n) { ws_send(ws, 1, data, n); }

static void on_open(void *ctx, WsConn *ws)
{
    NetTransport tr;
    (void)ctx;
    tr.ctx = ws, tr.send_text = send_text, tr.send_binary = send_binary;
    client_connected(&client, &tr);
}

static void on_message(void *ctx, int binary, const uint8_t *data, size_t n)
{
    (void)ctx;
    if (binary)
        client_on_binary(&client, data, n);
    else
        client_on_text(&client, (const char *)data, n);
}

static const char *on_close(void *ctx, int code, const char *reason, int attempt)
{
    NetLobby lobby;
    (void)ctx;
    if (!client_on_closed(&client, code, reason) || attempt >= REJOIN_ATTEMPTS)
        return NULL;
    client_lobby(&client, &lobby);
    snprintf(next_url, sizeof next_url, "%s/v1/rooms/%s", base, lobby.code);
    return next_url;
}

static int on_tick(void *ctx)
{
    (void)ctx;
    return client_tick(&client);
}

void net_init(const char *base_url, const char *player_name, const char *build_version, const char *iso_sha256)
{
    const char *url = base_url && *base_url ? base_url : getenv("DHD_NET_SERVER");
    size_t n;
    if (!inited) {
        InitializeCriticalSection(&api);
        client_init(&client, "", "", "");
        inited = 1;
    }
    snprintf(base, sizeof base, "%s", url ? url : "");
    n = strlen(base);
    while (n && base[n - 1] == '/')
        base[--n] = 0;
    snprintf(name, sizeof name, "%s", player_name);
    snprintf(version, sizeof version, "%s", build_version);
    snprintf(iso, sizeof iso, "%s", iso_sha256);
}

static int connect_to(const char *path)
{
    char url[300];
    WsHandler h = {NULL, on_open, on_message, on_close, on_tick};
    if (!inited)
        net_init(NULL, "Rider", "", "");
    net_leave();
    EnterCriticalSection(&api);
    client_free(&client);
    client_init(&client, name, version, iso);
    if (!base[0]) {
        client.lobby.state = NET_FAILED;
        snprintf(client.lobby.error, sizeof client.lobby.error, "no server: set DHD_NET_SERVER");
        LeaveCriticalSection(&api);
        return -1;
    }
    client.lobby.state = NET_CONNECTING;
    snprintf(url, sizeof url, "%s%s", base, path);
    conn = ws_start(url, &h);
    LeaveCriticalSection(&api);
    return 0;
}

int net_create_room(void)
{
    return connect_to("/v1/rooms/new");
}

int net_join_room(const char *code)
{
    static const char alphabet[] = "23456789ABCDEFGHJKMNPQRSTUVWXYZ";
    char path[32], up[8];
    int i;
    for (i = 0; code[i] && i < 7; i++)
        up[i] = (char)toupper((unsigned char)code[i]);
    up[i] = 0;
    if (i != 6 || strspn(up, alphabet) != 6)
        return -1;
    snprintf(path, sizeof path, "/v1/rooms/%s", up);
    return connect_to(path);
}

void net_leave(void)
{
    WsConn *c;
    if (!inited)
        return;
    EnterCriticalSection(&api);
    c = conn, conn = NULL;
    if (c) {
        client_leave(&client);
        ws_stop(c);
    }
    LeaveCriticalSection(&api);
}

int net_list_lobbies(NetLobbyInfo *out, int max)
{
    static char body[64 * 1024];
    char url[300], path[48];
    int i, n;
    snprintf(url, sizeof url, "%s/v1/lobbies", base);
    if (!base[0] || http_get(url, body, sizeof body) < 0 || (n = json_count(body, "lobbies")) < 0)
        return -1;
    if (n > max)
        n = max;
    for (i = 0; i < n; i++) {
        NetLobbyInfo *l = &out[i];
        long long v;
        memset(l, 0, sizeof *l);
#define FIELD(key) (snprintf(path, sizeof path, "lobbies[%d]." key, i), path)
        json_string(body, FIELD("code"), l->code, sizeof l->code);
        json_string(body, FIELD("roomName"), l->room_name, sizeof l->room_name);
        json_string(body, FIELD("version"), l->version, sizeof l->version);
        json_string(body, FIELD("hostName"), l->host_name, sizeof l->host_name);
        l->track = json_int(body, FIELD("track"), &v) ? (int)v : 0;
        l->players = json_int(body, FIELD("players"), &v) ? (int)v : 0;
        l->max_players = json_int(body, FIELD("maxPlayers"), &v) ? (int)v : 0;
#undef FIELD
    }
    return n;
}

void net_set_room(const NetRoomSettings *s) { client_set_room(&client, s); }
void net_pick(int member, int rider, int bike) { client_pick(&client, member, rider, bike); }
void net_set_ready(int ready) { client_ready(&client, ready); }
void net_kick(int member) { client_kick(&client, member); }
void net_start_race(void) { client_start(&client); }
void net_end_race(void) { client_end(&client); }

void net_get_lobby(NetLobby *out)
{
    if (!inited) {
        memset(out, 0, sizeof *out);
        return;
    }
    client_lobby(&client, out);
}

double net_seconds(void)
{
    return net_now_ms() / 1000.0;
}

double net_server_ms(void)
{
    return net_now_ms() + client.clock_offset;
}

int net_racing(void)
{
    return inited && client.racing;
}

int net_local_slot(void)
{
    return client.lobby.local_slot;
}

void net_submit_local(uint32_t field, const NetInput *in)
{
    client_submit_local(&client, field, in);
}

int net_inputs_for_field(uint32_t field, NetPad out[NET_MAX_SLOTS], unsigned timeout_ms)
{
    return inited ? client_inputs_for_field(&client, field, out, timeout_ms) : NET_ERROR;
}

uint32_t net_waiting_for(void)
{
    return inited ? client_waiting(&client) : 0;
}

void net_submit_hash(uint32_t field, uint64_t hash)
{
    client_submit_hash(&client, field, hash);
}

int net_send_frame(int type, const void *payload, size_t n)
{
    return inited && client_send_frame(&client, type, payload, n);
}

int net_receive_frame(uint8_t *out, size_t max)
{
    return inited ? client_receive_frame(&client, out, max) : 0;
}

int net_desync(uint32_t *field, uint32_t *slots)
{
    return inited && client_desync(&client, field, slots);
}
