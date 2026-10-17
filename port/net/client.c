/* One online player (client.h): docs/online-protocol.md's lobby messages, the race frames and
   pings, around the lockstep core. */
#include "client.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "../include/version.h"

enum { PING_LOBBY_MS = 1000, PING_RACE_MS = 500, RESEND_MS = 100, SILENT_MS = 10000 };

double net_now_ms(void)
{
    static LARGE_INTEGER freq;
    LARGE_INTEGER now;
    if (!freq.QuadPart)
        QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    return (double)now.QuadPart * 1000.0 / (double)freq.QuadPart;
}

static void copy(char *dst, size_t cap, const char *src)
{
    size_t n = strlen(src);
    if (n >= cap)
        n = cap - 1;
    memcpy(dst, src, n);
    dst[n] = 0;
}

static void changed(NetClient *c)
{
    c->lobby.seq++;
    WakeAllConditionVariable(&c->changed);
}

/* Sends with the lock held: the transports only queue. */
static void say(NetClient *c, const char *fmt, ...)
{
    char text[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(text, sizeof text, fmt, ap);
    va_end(ap);
    if (c->connected)
        c->tr.send_text(c->tr.ctx, text);
}

static void send_binary(NetClient *c, const uint8_t *data, size_t n)
{
    if (c->connected && n)
        c->tr.send_binary(c->tr.ctx, data, n);
}

void client_init(NetClient *c, const char *name, const char *version, const char *iso)
{
    memset(c, 0, sizeof *c);
    InitializeCriticalSection(&c->lock);
    InitializeConditionVariable(&c->changed);
    copy(c->name, sizeof c->name, name);
    {   /* the server takes 1-24 UTF-16 units */
        int units = 0;
        char *p;
        for (p = c->name; *p; p++)
            if ((*p & 0xC0) != 0x80 && (units += (*p & 0xF8) == 0xF0 ? 2 : 1) > 24)
                break;
        *p = 0;
    }
    copy(c->version, sizeof c->version, version);
    copy(c->iso, sizeof c->iso, iso);
    c->lobby.seed = -1;
    c->lobby.ping_ms = -1;
}

void client_free(NetClient *c)
{
    DeleteCriticalSection(&c->lock);
}

void client_connected(NetClient *c, const NetTransport *tr)
{
    char name[200];
    EnterCriticalSection(&c->lock);
    c->tr = *tr;
    c->connected = 1;
    c->last_rx = c->next_ping = net_now_ms();
    json_escape(name, sizeof name, c->name);
    /* clientVersion: this release (port/include/version.h), for the server's version gate */
    say(c, "{\"type\":\"hello\",\"protocol\":%d,\"version\":\"%s\",\"clientVersion\":\"%s\",\"isoSha256\":\"%s\","
        "\"name\":\"%s\"}", NET_PROTOCOL, c->version, DHD_VERSION, c->iso, name);
    if (c->lobby.state != NET_RECONNECTING)
        c->lobby.state = NET_CONNECTING;
    changed(c);
    LeaveCriticalSection(&c->lock);
}

static void read_room(NetClient *c, const char *t)
{
    NetLobby *l = &c->lobby;
    long long v;
    int i, n;
    json_string(t, "code", l->code, sizeof l->code);
    if (json_int(t, "hostId", &v))
        l->host_id = (int)v;
    l->is_host = l->host_id == l->member_id;
    json_string(t, "roomName", l->room_name, sizeof l->room_name);
    json_bool(t, "public", &l->is_public);
    json_bool(t, "started", &l->started);
    if (json_int(t, "maxPlayers", &v))
        l->max_players = (int)v;
    if (json_int(t, "settings.track", &v))
        l->track = (int)v;
    if (json_int(t, "settings.laps", &v))
        l->laps = (int)v;
    l->seed = json_int(t, "settings.seed", &v) ? v : -1;
    if (json_int(t, "inputDelay", &v))
        l->input_delay = (int)v;
    n = json_count(t, "members");
    l->nmembers = n < 0 ? 0 : n > NET_MAX_SLOTS ? NET_MAX_SLOTS : n;
    for (i = 0; i < l->nmembers; i++) {
        NetMember *m = &l->member[i];
        char path[32];
        memset(m, 0, sizeof *m);
        m->rtt_ms = -1;
#define FIELD(name) (snprintf(path, sizeof path, "members[%d]." name, i), path)
        if (json_int(t, FIELD("id"), &v))
            m->id = (int)v;
        json_string(t, FIELD("name"), m->name, sizeof m->name);
        json_bool(t, FIELD("ready"), &m->ready);
        if (json_int(t, FIELD("rider"), &v))
            m->rider = (int)v;
        if (json_int(t, FIELD("bike"), &v))
            m->bike = (int)v;
        if (json_int(t, FIELD("rttMs"), &v))
            m->rtt_ms = (int)v;
#undef FIELD
    }
}

static void read_start(NetClient *c, const char *t)
{
    NetLobby *l = &c->lobby;
    long long v;
    double server_time, start_at;
    int i, n, nslots = 0;
    l->local_slot = json_int(t, "yourSlot", &v) ? (int)v : 0;
    l->race_track = json_int(t, "track", &v) ? (int)v : 0;
    l->race_laps = json_int(t, "laps", &v) ? (int)v : 0;
    l->race_seed = json_int(t, "seed", &v) ? (uint32_t)v : 0;
    if (json_int(t, "inputDelay", &v))
        l->input_delay = (int)v;
    memset(l->racer, 0, sizeof l->racer);
    n = json_count(t, "players");
    for (i = 0; i < n; i++) {
        char path[32];
        int slot;
        snprintf(path, sizeof path, "players[%d].slot", i);
        if (!json_int(t, path, &v) || v < 0 || v >= NET_MAX_SLOTS)
            continue;
        slot = (int)v;
        l->racer[slot].slot = slot;
        snprintf(path, sizeof path, "players[%d].memberId", i);
        if (json_int(t, path, &v))
            l->racer[slot].member_id = (int)v;
        snprintf(path, sizeof path, "players[%d].name", i);
        json_string(t, path, l->racer[slot].name, sizeof l->racer[slot].name);
        snprintf(path, sizeof path, "players[%d].rider", i);
        if (json_int(t, path, &v))
            l->racer[slot].rider = (int)v;
        snprintf(path, sizeof path, "players[%d].bike", i);
        if (json_int(t, path, &v))
            l->racer[slot].bike = (int)v;
        if (slot + 1 > nslots)
            nslots = slot + 1;
    }
    l->nslots = nslots;
    /* field 0's time on our clock */
    if (json_number(t, "startAt", &start_at)) {
        if (!c->have_offset && json_number(t, "serverTime", &server_time))
            c->clock_offset = server_time - net_now_ms();
        l->start_at = (start_at - c->clock_offset) / 1000.0;
    } else
        l->start_at = net_now_ms() / 1000.0;
    ls_init(&c->ls, nslots, l->local_slot, l->input_delay);  /* every slot a racer */
    ls_start(&c->ls);
    c->racing = 1;
    c->waiting = 0;
    c->stall_since = 0;
    c->desynced = 0;
    memset(c->sent_hash_field, 0xFF, sizeof c->sent_hash_field);
    l->offline = 0;
    l->started = 1;
    l->state = NET_RACING;
}

static void read_desync(NetClient *c, const char *t)
{
    long long field;
    uint32_t slots = 0;
    char ours[20] = "", hash[20], path[32];
    int s, at;
    if (!json_int(t, "field", &field))
        return;
    at = (int)((uint32_t)field / NET_HASH_PERIOD % LS_HASHES);
    if (c->sent_hash_field[at] == (uint32_t)field)
        snprintf(ours, sizeof ours, "%016llx", (unsigned long long)c->sent_hash[at]);
    for (s = 0; s < NET_MAX_SLOTS; s++) {
        snprintf(path, sizeof path, "hashes.%d", s);
        if (json_string(t, path, hash, sizeof hash) && s != c->lobby.local_slot &&
            (!ours[0] || _stricmp(hash, ours)))
            slots |= 1u << s;
    }
    if (!c->desynced || (uint32_t)field < c->desync_field) {
        c->desynced = 1;
        c->desync_field = (uint32_t)field;
        c->desync_slots = slots;
    }
}

static void add_rtt(NetClient *c, double rtt)
{
    float sorted[5];
    int i, j, n;
    if (rtt < 0 || rtt > 60000)
        return;
    memmove(c->rtt + 1, c->rtt, sizeof c->rtt - sizeof c->rtt[0]);
    c->rtt[0] = (float)rtt;
    if (c->nrtt < 5)
        c->nrtt++;
    n = c->nrtt;
    memcpy(sorted, c->rtt, sizeof sorted);
    for (i = 1; i < n; i++)
        for (j = i; j > 0 && sorted[j] < sorted[j - 1]; j--) {
            float x = sorted[j];
            sorted[j] = sorted[j - 1], sorted[j - 1] = x;
        }
    c->lobby.ping_ms = (int)(sorted[n / 2] + 0.5f);
}

void client_on_text(NetClient *c, const char *text, size_t n)
{
    char type[24];
    long long v;
    double t, server_time;
    (void)n;
    EnterCriticalSection(&c->lock);
    c->last_rx = net_now_ms();
    if (!json_string(text, "type", type, sizeof type))
        ;
    else if (!strcmp(type, "ping") && json_int(text, "s", &v))
        say(c, "{\"type\":\"pong\",\"s\":%lld}", v);
    else if (!strcmp(type, "pong") && json_number(text, "t", &t)) {
        double now = net_now_ms(), rtt = now - t;
        add_rtt(c, rtt);
        if (json_number(text, "serverTime", &server_time) &&
            (!c->have_offset || rtt <= c->lobby.ping_ms))
            c->clock_offset = server_time - (t + rtt / 2), c->have_offset = 1;
        changed(c);
    } else if (!strcmp(type, "welcome")) {
        json_string(text, "code", c->lobby.code, sizeof c->lobby.code);
        if (json_int(text, "memberId", &v))
            c->lobby.member_id = (int)v;
        json_bool(text, "host", &c->lobby.is_host);
        if (c->lobby.is_host)
            c->lobby.host_id = c->lobby.member_id;
        if (json_number(text, "serverTime", &server_time) && !c->have_offset)
            c->clock_offset = server_time - net_now_ms();
        c->lobby.state = NET_LOBBY;
        c->lobby.error_code[0] = c->lobby.error[0] = 0;
        if (c->picked)  /* after a reconnect: what we had chosen */
            say(c, "{\"type\":\"pick\",\"rider\":%d,\"bike\":%d}", c->rider, c->bike);
        if (c->ready)
            say(c, "{\"type\":\"ready\",\"ready\":true}");
        changed(c);
    } else if (!strcmp(type, "room")) {
        read_room(c, text);
        changed(c);
    } else if (!strcmp(type, "start")) {
        read_start(c, text);
        changed(c);
    } else if (!strcmp(type, "left") && json_int(text, "slot", &v) && v >= 0 && v < NET_MAX_SLOTS) {
        long long from;
        if (!json_int(text, "fromField", &from))  /* null: the room was evicted mid-race */
            from = c->racing ? ls_after_last_received(&c->ls, (int)v) : 0;
        if (c->racing)
            ls_slot_ai_from(&c->ls, (int)v, (uint32_t)from);
        c->lobby.racer[v].left = 1;
        c->lobby.racer[v].left_field = (uint32_t)from;
        changed(c);
    } else if (!strcmp(type, "desync")) {
        read_desync(c, text);
        changed(c);
    } else if (!strcmp(type, "ended")) {
        c->racing = 0;
        c->ready = 0;
        c->lobby.started = 0;
        c->lobby.state = NET_LOBBY;
        changed(c);
    } else if (!strcmp(type, "error")) {
        json_string(text, "code", c->lobby.error_code, sizeof c->lobby.error_code);
        json_string(text, "message", c->lobby.error, sizeof c->lobby.error);
        changed(c);
    }
    LeaveCriticalSection(&c->lock);
}

void client_on_binary(NetClient *c, const uint8_t *data, size_t n)
{
    EnterCriticalSection(&c->lock);
    c->last_rx = net_now_ms();
    if (n >= 2 + sizeof(double) && data[0] == LS_MSG_PONG) {
        double sent;
        memcpy(&sent, data + 2, sizeof sent);
        add_rtt(c, net_now_ms() - sent);
    } else if (c->racing && n >= 2 && n <= NET_FRAME_MAX && (data[0] == NET_FRAME_STATE || data[0] == NET_FRAME_EVENT)) {
        unsigned at = (c->relayed_head + c->relayed_count) % NET_RELAY_QUEUE;
        if (c->relayed_count == NET_RELAY_QUEUE)
            c->relayed_head = (c->relayed_head + 1) % NET_RELAY_QUEUE;
        else
            c->relayed_count++;
        memcpy(c->relayed[at], data, n);
        c->relayed_size[at] = (uint16_t)n;
    } else if (c->racing) {
        LsEvent ev = ls_receive(&c->ls, data, n);
        if (ev.fresh)
            WakeAllConditionVariable(&c->changed);
    }
    LeaveCriticalSection(&c->lock);
}

int client_on_closed(NetClient *c, int code, const char *reason)
{
    int rejoin = 0;
    EnterCriticalSection(&c->lock);
    c->connected = 0;
    if (c->racing) {
        /* No reconnect during a race (protocol 1): the others hand our rider to the AI, and
           here everyone else becomes the AI from the first field we lack, so the race goes on. */
        int s;
        for (s = 0; s < c->ls.nslots; s++)
            if (s != c->ls.local)
                ls_slot_ai_from(&c->ls, s, (uint32_t)(ls_have_through(&c->ls, s) + 1));
        c->lobby.offline = 1;
        c->lobby.state = NET_FAILED;
    } else if (c->leaving)
        c->lobby.state = NET_IDLE;
    else if (code >= 4000 && code < 4100)  /* refused */
        c->lobby.state = NET_FAILED;
    else if (c->lobby.code[0] && c->lobby.state != NET_CONNECTING) {
        c->lobby.state = NET_RECONNECTING;
        rejoin = 1;
    } else
        c->lobby.state = NET_FAILED;
    if (!c->lobby.error[0] && c->lobby.state == NET_FAILED)
        snprintf(c->lobby.error, sizeof c->lobby.error, "connection closed (%d%s%s)", code,
                 reason && *reason ? ": " : "", reason ? reason : "");
    changed(c);
    LeaveCriticalSection(&c->lock);
    return rejoin;
}

int client_tick(NetClient *c)
{
    double now = net_now_ms();
    int dead = 0;
    EnterCriticalSection(&c->lock);
    if (c->connected) {
        if (now >= c->next_ping) {
            if (c->racing) {
                uint8_t ping[2 + sizeof(double)] = {LS_MSG_PING, (uint8_t)c->ls.local};
                memcpy(ping + 2, &now, sizeof now);
                send_binary(c, ping, sizeof ping);
                c->next_ping = now + PING_RACE_MS;
            } else {
                if (c->lobby.state == NET_LOBBY)
                    say(c, "{\"type\":\"ping\",\"t\":%.3f}", now);
                c->next_ping = now + PING_LOBBY_MS;
            }
        }
        if (c->racing && c->stall_since && now - c->stall_since > RESEND_MS &&
            now - c->last_resend > RESEND_MS) {
            uint8_t frame[LS_MAX_FRAME];
            send_binary(c, frame, ls_encode_resend(&c->ls, frame, sizeof frame));
            c->last_resend = now;
        }
        dead = now - c->last_rx > SILENT_MS;
    }
    LeaveCriticalSection(&c->lock);
    return dead;
}

void client_set_room(NetClient *c, const NetRoomSettings *s)
{
    char text[512], name[200];
    int n = snprintf(text, sizeof text, "{\"type\":\"settings\"");
    if (s->track >= 0)
        n += snprintf(text + n, sizeof text - n, ",\"track\":%d", s->track);
    if (s->laps >= 0)
        n += snprintf(text + n, sizeof text - n, ",\"laps\":%d", s->laps);
    if (s->seed >= 0)
        n += snprintf(text + n, sizeof text - n, ",\"seed\":%lld", s->seed);
    if (s->is_public >= 0)
        n += snprintf(text + n, sizeof text - n, ",\"public\":%s", s->is_public ? "true" : "false");
    if (s->max_players >= 0)
        n += snprintf(text + n, sizeof text - n, ",\"maxPlayers\":%d", s->max_players);
    if (s->room_name) {
        json_escape(name, sizeof name, s->room_name);
        n += snprintf(text + n, sizeof text - n, ",\"roomName\":\"%s\"", name);
    }
    snprintf(text + n, sizeof text - n, "}");
    EnterCriticalSection(&c->lock);
    say(c, "%s", text);
    LeaveCriticalSection(&c->lock);
}

void client_pick(NetClient *c, int member, int rider, int bike)
{
    EnterCriticalSection(&c->lock);
    if (member < 0 || member == c->lobby.member_id) {
        c->picked = 1, c->rider = rider, c->bike = bike;
        say(c, "{\"type\":\"pick\",\"rider\":%d,\"bike\":%d}", rider, bike);
    } else
        say(c, "{\"type\":\"pick\",\"member\":%d,\"rider\":%d,\"bike\":%d}", member, rider, bike);
    LeaveCriticalSection(&c->lock);
}

void client_ready(NetClient *c, int ready)
{
    EnterCriticalSection(&c->lock);
    c->ready = ready != 0;
    say(c, "{\"type\":\"ready\",\"ready\":%s}", ready ? "true" : "false");
    LeaveCriticalSection(&c->lock);
}

void client_kick(NetClient *c, int member)
{
    EnterCriticalSection(&c->lock);
    say(c, "{\"type\":\"kick\",\"member\":%d}", member);
    LeaveCriticalSection(&c->lock);
}

void client_start(NetClient *c)
{
    EnterCriticalSection(&c->lock);
    say(c, "{\"type\":\"start\"}");
    LeaveCriticalSection(&c->lock);
}

void client_end(NetClient *c)
{
    EnterCriticalSection(&c->lock);
    say(c, "{\"type\":\"end\"}");
    LeaveCriticalSection(&c->lock);
}

void client_leave(NetClient *c)
{
    EnterCriticalSection(&c->lock);
    c->leaving = 1;
    if (c->racing)
        say(c, "{\"type\":\"leave\"}");
    LeaveCriticalSection(&c->lock);
}

void client_lobby(NetClient *c, NetLobby *out)
{
    EnterCriticalSection(&c->lock);
    *out = c->lobby;
    LeaveCriticalSection(&c->lock);
}

void client_submit_local(NetClient *c, uint32_t field, const NetInput *in)
{
    EnterCriticalSection(&c->lock);
    if (c->racing && ls_submit_local(&c->ls, field, in) > 0) {
        uint8_t frame[LS_MAX_FRAME];
        send_binary(c, frame, ls_encode_recent(&c->ls, frame, sizeof frame));
    }
    LeaveCriticalSection(&c->lock);
}

int client_inputs_for_field(NetClient *c, uint32_t field, NetPad out[NET_MAX_SLOTS], unsigned timeout_ms)
{
    double deadline = net_now_ms() + timeout_ms;
    int result;
    EnterCriticalSection(&c->lock);
    for (;;) {
        double now = net_now_ms();
        if (!c->racing) {
            result = NET_ERROR;
            break;
        }
        if (ls_take(&c->ls, field, out) == 0) {
            c->waiting = 0;
            c->stall_since = 0;
            result = NET_OK;
            break;
        }
        c->waiting = ls_missing(&c->ls, field);
        if (!c->stall_since)
            c->stall_since = now;
        if (now >= deadline) {
            result = NET_WAIT;
            break;
        }
        SleepConditionVariableCS(&c->changed, &c->lock, (DWORD)ceil(deadline - now));
    }
    LeaveCriticalSection(&c->lock);
    return result;
}

uint32_t client_waiting(NetClient *c)
{
    uint32_t mask;
    EnterCriticalSection(&c->lock);
    mask = c->waiting;
    LeaveCriticalSection(&c->lock);
    return mask;
}

void client_submit_hash(NetClient *c, uint32_t field, uint64_t hash)
{
    EnterCriticalSection(&c->lock);
    if (c->racing) {
        ls_local_hash(&c->ls, field, hash);
        if (net_hash_due(field)) {
            uint8_t frame[LS_HASH_SIZE];
            int at = (int)(field / NET_HASH_PERIOD % LS_HASHES);
            c->sent_hash[at] = hash;
            c->sent_hash_field[at] = field;
            send_binary(c, frame, ls_encode_hash(c->ls.local, field, hash, frame));
        }
    }
    LeaveCriticalSection(&c->lock);
}

int client_send_frame(NetClient *c, int type, const void *payload, size_t n)
{
    uint8_t frame[NET_FRAME_MAX];
    int sent = 0;
    if (n > sizeof frame - 2) return 0;
    frame[0] = (uint8_t)type;
    frame[1] = 0;
    memcpy(frame + 2, payload, n);
    EnterCriticalSection(&c->lock);
    if (c->racing && c->connected) {
        send_binary(c, frame, n + 2);
        sent = 1;
    }
    LeaveCriticalSection(&c->lock);
    return sent;
}

int client_receive_frame(NetClient *c, uint8_t *out, size_t max)
{
    int n = 0;
    EnterCriticalSection(&c->lock);
    if (c->relayed_count) {
        unsigned at = c->relayed_head;
        n = c->relayed_size[at] <= max ? c->relayed_size[at] : 0;
        memcpy(out, c->relayed[at], n);
        c->relayed_head = (at + 1) % NET_RELAY_QUEUE;
        c->relayed_count--;
    }
    LeaveCriticalSection(&c->lock);
    return n;
}

int client_desync(NetClient *c, uint32_t *field, uint32_t *slots)
{
    int d;
    EnterCriticalSection(&c->lock);
    d = c->desynced || c->ls.desynced;
    if (c->desynced)
        *field = c->desync_field, *slots = c->desync_slots;
    else if (c->ls.desynced)  /* found here: only when the hashes travel peer to peer */
        *field = c->ls.desync_field, *slots = c->ls.desync_slots;
    LeaveCriticalSection(&c->lock);
    return d;
}
