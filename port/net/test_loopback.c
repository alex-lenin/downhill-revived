/* Four players in one process: client.c over a fake transport, through a fake relay that does
   what online/server does (docs/online-protocol.md): welcome and room snapshots, slots at start,
   INPUT relayed with the slot byte overwritten, PING answered, HASH compared, left with
   fromField. Each player has its own one-way latency; delivery keeps each socket's order.
   Each player's game thread runs a toy race whose state depends on every pad of every field. */
#include "client.h"
#include "json.h"

#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int failures;
#define CHECK(c) do { if (!(c)) { printf("%s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #c); failures++; } } while (0)

enum { PLAYERS = 4, FIELDS = 600 };

typedef struct Msg { struct Msg *next; double due; int binary; size_t n; char data[]; } Msg;

typedef struct {
    NetClient client;
    int index, latency_ms;     /* one way, to the relay */
    int open, member, slot;
    int ready, rider, bike;
    Msg *head, *tail;          /* to deliver, in order */
    double last_due;
    HANDLE thread, game;
    volatile LONG stop;
    /* the toy race */
    int start_late_ms, drop_at, corrupt_from;
    uint32_t played;
    uint64_t state;
    uint32_t saw_waiting;
    int pad_kinds[PLAYERS];
} Player;

static Player players[PLAYERS];
static CRITICAL_SECTION relay;
static int next_member, host_member, started, null_from_field, race_delay;
static int64_t last_field[PLAYERS];
static struct { uint32_t field; uint8_t have; uint64_t hash[PLAYERS]; } hashes[64];
static int64_t reported_desync = -1;

static void deliver(Player *to, const Player *from, int binary, const void *data, size_t n)
{
    Msg *m = malloc(sizeof *m + n + 1);
    double due = net_now_ms() + from->latency_ms + to->latency_ms;
    if (!to->open)
        return free(m);
    if (due < to->last_due)
        due = to->last_due;  /* one socket: in order */
    to->last_due = due;
    m->next = NULL, m->due = due, m->binary = binary, m->n = n;
    memcpy(m->data, data, n);
    m->data[n] = 0;
    if (to->tail)
        to->tail->next = m;
    else
        to->head = m;
    to->tail = m;
}

static void text_to(Player *to, const Player *from, const char *text)
{
    deliver(to, from, 0, text, strlen(text));
}

static void send_room(void)
{
    char text[2048];
    int n, i, first = 1;
    n = snprintf(text, sizeof text, "{\"type\":\"room\",\"code\":\"TEST22\",\"hostId\":%d,\"roomName\":\"loop\","
                 "\"public\":false,\"maxPlayers\":4,\"started\":%s,\"version\":\"test\",\"settings\":{\"track\":3,"
                 "\"laps\":2,\"seed\":null},\"members\":[", host_member, started ? "true" : "false");
    for (i = 0; i < PLAYERS; i++)
        if (players[i].open && players[i].member >= 0) {
            n += snprintf(text + n, sizeof text - n, "%s{\"id\":%d,\"name\":\"P%d\",\"ready\":%s,\"rider\":%d,"
                          "\"bike\":%d,\"rttMs\":%d}", first ? "" : ",", players[i].member, i,
                          players[i].ready ? "true" : "false", players[i].rider, players[i].bike,
                          players[i].latency_ms * 2);
            first = 0;
        }
    snprintf(text + n, sizeof text - n, "],\"inputDelay\":3}");
    for (i = 0; i < PLAYERS; i++)
        text_to(&players[i], &players[i], text);
}

static void start_race(void)
{
    float rtt[PLAYERS];
    char text[2048];
    int i, slot = 0, delay;
    double now = (double)time(NULL) * 1000;
    for (i = 0; i < PLAYERS; i++) {
        rtt[i] = (float)players[i].latency_ms * 2;
        players[i].slot = slot++;
        last_field[players[i].slot] = -1;
    }
    delay = race_delay = ls_auto_delay(rtt, PLAYERS);
    started = 1;
    for (i = 0; i < PLAYERS; i++) {
        int n = snprintf(text, sizeof text, "{\"type\":\"start\",\"yourSlot\":%d,\"track\":3,\"laps\":2,"
                         "\"seed\":2864434397,\"inputDelay\":%d,\"players\":[", players[i].slot, delay), j;
        for (j = 0; j < PLAYERS; j++)
            n += snprintf(text + n, sizeof text - n, "%s{\"slot\":%d,\"memberId\":%d,\"name\":\"P%d\",\"rider\":%d,"
                          "\"bike\":%d}", j ? "," : "", players[j].slot, players[j].member, j, players[j].rider,
                          players[j].bike);
        snprintf(text + n, sizeof text - n, "],\"serverTime\":%.0f,\"startAt\":%.0f}", now, now + 300);
        text_to(&players[i], &players[i], text);
    }
    printf("  started: input delay %d\n", delay);
}

static void relay_text(Player *p, const char *text)
{
    char type[24], reply[256];
    long long v;
    double t;
    json_string(text, "type", type, sizeof type);
    if (!strcmp(type, "hello")) {
        char name[64], iso[80];
        CHECK(json_int(text, "protocol", &v) && v == NET_PROTOCOL);
        CHECK(json_string(text, "isoSha256", iso, sizeof iso) && strlen(iso) == 64);
        CHECK(json_string(text, "name", name, sizeof name) && name[0] == 'P');
        p->member = next_member++;
        if (host_member < 0)
            host_member = p->member;
        snprintf(reply, sizeof reply, "{\"type\":\"welcome\",\"protocol\":1,\"code\":\"TEST22\",\"memberId\":%d,"
                 "\"host\":%s,\"serverTime\":1}", p->member, p->member == host_member ? "true" : "false");
        text_to(p, p, reply);
        send_room();
    } else if (!strcmp(type, "ready") && json_bool(text, "ready", &p->ready))
        send_room();
    else if (!strcmp(type, "pick")) {
        CHECK(!json_find(text, "member"));
        json_int(text, "rider", &v), p->rider = (int)v;
        json_int(text, "bike", &v), p->bike = (int)v;
        send_room();
    } else if (!strcmp(type, "start")) {
        int i, all = 1;
        CHECK(p->member == host_member);
        for (i = 0; i < PLAYERS; i++)
            all &= players[i].ready;
        if (all)
            start_race();
        else
            text_to(p, p, "{\"type\":\"error\",\"code\":\"not_ready\",\"message\":\"Not ready.\"}");
    } else if (!strcmp(type, "ping") && json_number(text, "t", &t)) {
        snprintf(reply, sizeof reply, "{\"type\":\"pong\",\"t\":%.3f,\"serverTime\":%.0f}", t, (double)time(NULL) * 1000);
        text_to(p, p, reply);
    } else if (!strcmp(type, "pong") || !strcmp(type, "leave"))
        ;
    else {
        printf("  relay: unexpected %s\n", text);
        failures++;
    }
}

static void relay_binary(Player *p, const uint8_t *data, size_t n)
{
    uint8_t frame[256];
    int i;
    CHECK(n >= 2 && n <= 256);
    memcpy(frame, data, n);
    CHECK(frame[1] == p->slot);  /* (the server would overwrite it) */
    frame[1] = (uint8_t)p->slot;
    if (frame[0] == LS_MSG_INPUT) {
        uint32_t first = frame[2] | frame[3] << 8 | frame[4] << 16 | (uint32_t)frame[5] << 24;
        CHECK(frame[6] >= 1 && frame[6] <= 16 && n == 7 + 6u * frame[6]);
        CHECK(first >= (uint32_t)race_delay);  /* the neutral fields aren't sent */
        if ((int64_t)first + frame[6] - 1 > last_field[p->slot])
            last_field[p->slot] = first + frame[6] - 1;
        for (i = 0; i < PLAYERS; i++)
            if (&players[i] != p)
                deliver(&players[i], p, 1, frame, n);
    } else if (frame[0] == LS_MSG_PING) {
        frame[0] = LS_MSG_PONG;
        deliver(p, p, 1, frame, n);
    } else if (frame[0] == LS_MSG_HASH) {
        uint32_t field = frame[2] | frame[3] << 8 | frame[4] << 16 | (uint32_t)frame[5] << 24;
        uint64_t hash;
        int at = field / 60 % 64, have = 0, differ = 0;
        CHECK(n == 14 && field % 60 == 59);
        memcpy(&hash, frame + 6, 8);
        if (hashes[at].field != field)
            memset(&hashes[at], 0, sizeof hashes[at]), hashes[at].field = field;
        hashes[at].hash[p->slot] = hash;
        hashes[at].have |= 1 << p->slot;
        for (i = 0; i < PLAYERS; i++)
            if (players[i].open && !(hashes[at].have & 1 << players[i].slot))
                have = -1;
        for (i = 0; i < PLAYERS && !have; i++)
            if (hashes[at].have & 1 << i && hashes[at].hash[i] != hash)
                differ = 1;
        if (!have && differ && (reported_desync < 0 || field < reported_desync)) {
            char text[512];
            int k = snprintf(text, sizeof text, "{\"type\":\"desync\",\"field\":%u,\"hashes\":{", field), first = 1;
            reported_desync = field;
            for (i = 0; i < PLAYERS; i++)
                if (hashes[at].have & 1 << i)
                    k += snprintf(text + k, sizeof text - k, "%s\"%d\":\"%016llx\"", first ? "" : ",", i,
                                  (unsigned long long)hashes[at].hash[i]), first = 0;
            snprintf(text + k, sizeof text - k, "}}");
            for (i = 0; i < PLAYERS; i++)
                text_to(&players[i], &players[i], text);
        }
    }
}

static void tr_text(void *ctx, const char *text)
{
    EnterCriticalSection(&relay);
    relay_text(ctx, text);
    LeaveCriticalSection(&relay);
}

static void tr_binary(void *ctx, const void *data, size_t n)
{
    EnterCriticalSection(&relay);
    if (((Player *)ctx)->open)
        relay_binary(ctx, data, n);
    LeaveCriticalSection(&relay);
}

/* The socket closes: the others hear left, after every frame already relayed. */
static void drop(Player *p)
{
    char text[160];
    int i;
    EnterCriticalSection(&relay);
    p->open = 0;
    if (null_from_field)
        snprintf(text, sizeof text, "{\"type\":\"left\",\"slot\":%d,\"memberId\":%d,\"fromField\":null,"
                 "\"reason\":\"disconnect\"}", p->slot, p->member);
    else
        snprintf(text, sizeof text, "{\"type\":\"left\",\"slot\":%d,\"memberId\":%d,\"fromField\":%lld,"
                 "\"reason\":\"disconnect\"}", p->slot, p->member, (long long)(last_field[p->slot] + 1));
    for (i = 0; i < PLAYERS; i++)
        if (players[i].open)
            text_to(&players[i], p, text);
    LeaveCriticalSection(&relay);
    client_on_closed(&p->client, 1006, "dropped");
}

/* The network thread: deliveries when due, and ticks. */
static DWORD WINAPI network(void *arg)
{
    Player *p = arg;
    while (!p->stop) {
        Msg *m = NULL;
        EnterCriticalSection(&relay);
        if (p->head && p->head->due <= net_now_ms()) {
            m = p->head;
            p->head = m->next;
            if (!p->head)
                p->tail = NULL;
        }
        LeaveCriticalSection(&relay);
        if (m) {
            if (p->open) {
                if (m->binary)
                    client_on_binary(&p->client, (uint8_t *)m->data, m->n);
                else
                    client_on_text(&p->client, m->data, m->n);
            }
            free(m);
            continue;
        }
        client_tick(&p->client);
        Sleep(1);
    }
    return 0;
}

static NetInput pad_of(int slot, uint32_t field)
{
    NetInput in;
    uint32_t x = (field + 7) * 2654435761u ^ (uint32_t)(slot + 1) * 0x9E3779B9u;
    in.buttons = (uint16_t)x, in.lx = (uint8_t)(x >> 16), in.ly = (uint8_t)(x >> 24);
    in.rx = (uint8_t)(x >> 5), in.ry = (uint8_t)(x >> 11);
    return in;
}

/* The game thread: one field after another, as pad_vsync will. */
static DWORD WINAPI game(void *arg)
{
    Player *p = arg;
    NetLobby lobby;
    uint32_t f;
    double give_up;
    Sleep(p->start_late_ms);
    client_lobby(&p->client, &lobby);
    p->state = lobby.race_seed;
    for (f = 0; f < FIELDS; f++) {
        NetPad out[NET_MAX_SLOTS];
        NetInput in = pad_of(lobby.local_slot, f);
        int r, s;
        if (p->drop_at && f == (uint32_t)p->drop_at)
            drop(p);
        client_submit_local(&p->client, f, &in);
        give_up = net_now_ms() + 5000;
        while ((r = client_inputs_for_field(&p->client, f, out, 20)) == NET_WAIT && net_now_ms() < give_up)
            p->saw_waiting |= client_waiting(&p->client);
        if (r != NET_OK) {
            printf("  player %d stuck at field %u (%d), waiting for %x\n", p->index, f, r, client_waiting(&p->client));
            failures++;
            return 1;
        }
        for (s = 0; s < lobby.nslots; s++) {
            p->state = net_hash64(&out[s], sizeof out[s], p->state);
            if (out[s].kind != (p->pad_kinds[s] & 0xFF))
                p->pad_kinds[s] = out[s].kind | (int)f << 8;  /* the kind, and the field it changed */
        }
        if (net_hash_due(f))
            client_submit_hash(&p->client, f, p->state ^ (p->corrupt_from && f >= (uint32_t)p->corrupt_from));
        p->played = f + 1;
    }
    return 0;
}

static void run(const char *name, const int latency[PLAYERS], int late_player, int dropper, int corrupter)
{
    int i;
    NetTransport tr;
    double until;
    NetLobby lobby;
    printf("%s\n", name);
    memset(players, 0, sizeof players);
    next_member = 0, host_member = -1, started = 0, reported_desync = -1;
    memset(hashes, 0, sizeof hashes);
    for (i = 0; i < PLAYERS; i++) {
        Player *p = &players[i];
        char who[8];
        snprintf(who, sizeof who, "P%d", i);
        p->index = i, p->latency_ms = latency[i], p->member = -1, p->open = 1;
        client_init(&p->client, who, "test", "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");
        p->thread = CreateThread(NULL, 0, network, p, 0, NULL);
    }
    for (i = 0; i < PLAYERS; i++) {  /* join in order: player 0 hosts */
        tr.ctx = &players[i], tr.send_text = tr_text, tr.send_binary = tr_binary;
        client_connected(&players[i].client, &tr);
        Sleep(30);
    }
    for (i = 0; i < PLAYERS; i++) {
        client_pick(&players[i].client, -1, i + 1, i % 2);
        client_ready(&players[i].client, 1);
    }
    until = net_now_ms() + 3000;
    do {
        Sleep(10);
        client_lobby(&players[0].client, &lobby);
    } while ((lobby.nmembers < PLAYERS || !lobby.member[PLAYERS - 1].ready) && net_now_ms() < until);
    CHECK(lobby.state == NET_LOBBY && lobby.is_host && lobby.nmembers == PLAYERS);
    CHECK(lobby.member[2].rider == 3 && lobby.member[3].bike == 1 && lobby.member[1].rtt_ms == latency[1] * 2);
    client_start(&players[0].client);
    until = net_now_ms() + 3000;
    for (i = 0; i < PLAYERS; i++) {
        do {
            Sleep(5);
            client_lobby(&players[i].client, &lobby);
        } while (lobby.state != NET_RACING && net_now_ms() < until);
        CHECK(lobby.state == NET_RACING && lobby.nslots == PLAYERS && lobby.local_slot == i);
        CHECK(lobby.racer[2].rider == 3 && lobby.race_seed == 2864434397u && lobby.input_delay >= 1);
    }
    for (i = 0; i < PLAYERS; i++) {
        Player *p = &players[i];
        p->start_late_ms = i == late_player ? 700 : 0;
        p->drop_at = i == dropper ? 300 : 0;
        p->corrupt_from = i == corrupter ? 400 : 0;
        p->game = CreateThread(NULL, 0, game, p, 0, NULL);
    }
    for (i = 0; i < PLAYERS; i++) {
        WaitForSingleObject(players[i].game, 30000);
        CloseHandle(players[i].game);
    }
    for (i = 0; i < PLAYERS; i++) {
        Player *p = &players[i];
        uint32_t field = 0, slots = 0;
        int desync = client_desync(&p->client, &field, &slots);
        client_lobby(&p->client, &lobby);
        CHECK(p->played == FIELDS);
        printf("  player %d: %u fields, state %016llx, ping %d ms, waited for %x%s", i, p->played,
               (unsigned long long)p->state, lobby.ping_ms, p->saw_waiting, lobby.offline ? ", offline" : "");
        if (desync)
            printf(", desync at %u with %x", field, slots);
        printf("\n");
        if (i != dropper)
            CHECK(lobby.ping_ms >= latency[i] * 2 && lobby.ping_ms < latency[i] * 2 + 40);
        if (i != dropper && dropper >= 0) {
            CHECK((players[i].pad_kinds[dropper] & 0xFF) == NET_SLOT_AI);
            CHECK(players[i].pad_kinds[dropper] >> 8 == (players[0].pad_kinds[dropper] >> 8));
            CHECK(lobby.racer[dropper].left);
        }
        if (i != dropper)  /* (the dropper is never player 0) */
            CHECK(p->state == players[0].state);
        if (i == dropper)
            CHECK(lobby.offline && lobby.state == NET_FAILED);
        if (late_player >= 0 && i != late_player)
            CHECK(p->saw_waiting & 1u << late_player);
        if (corrupter >= 0) {
            CHECK(desync && field == 419);
            CHECK(slots == (i == corrupter ? 0xFu & ~(1u << i) : 1u << corrupter));
        } else
            CHECK(!desync);
    }
    if (dropper >= 0)
        printf("  slot %d: the AI from field %d on every PC\n", dropper,
               players[dropper ? 0 : 1].pad_kinds[dropper] >> 8);
    for (i = 0; i < PLAYERS; i++) {
        Msg *m;
        players[i].stop = 1;
        WaitForSingleObject(players[i].thread, INFINITE);
        CloseHandle(players[i].thread);
        while ((m = players[i].head))
            players[i].head = m->next, free(m);
        client_free(&players[i].client);
    }
}

int main(void)
{
    static const int lan[PLAYERS] = {2, 3, 2, 4}, wan[PLAYERS] = {10, 25, 45, 70};
    timeBeginPeriod(1);
    InitializeCriticalSection(&relay);
    run("four players, close", lan, -1, -1, -1);
    run("four players, 10-70 ms away", wan, -1, -1, -1);
    run("a player starts late", wan, 2, -1, -1);
    run("a player drops at field 300", wan, -1, 3, -1);
    null_from_field = 1;
    run("a player drops, the server lost track (fromField null)", wan, -1, 1, -1);
    null_from_field = 0;
    run("a player's state differs from field 400", lan, -1, -1, 2);
    timeEndPeriod(1);
    if (failures) {
        printf("test_loopback: %d failures\n", failures);
        return 1;
    }
    printf("test_loopback: ok\n");
    return 0;
}
