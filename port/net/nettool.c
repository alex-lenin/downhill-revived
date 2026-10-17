/* A command-line player for testing the relay end to end (run_e2e.py starts four of them).
   It joins a room through net.h like the game will, then plays a toy race paced at 59.94
   fields per second whose state depends on every pad of every field, and prints a RESULT line.

   usage: nettool [--server URL] [--name NAME] [--fields N] [--drop-at F] [--corrupt-from F]
                  [--bot --version V --iso SHA256 [--replay FILE --rider N --seconds S]]
                  host [--players N] [--code-file PATH] | join CODE | list
   The server is --server or DHD_NET_SERVER (ws://127.0.0.1:8787 for wrangler dev).
   --bot joins a room of real games (finding 041): with the game's version and ISO hash it takes a
   slot and rides it (Cross held, steering in a slow rhythm), sends no state hashes of its own
   and waits as long as a loading game needs. */
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net.h"

static void wait_for(int (*done)(const NetLobby *), double seconds, const char *what)
{
    double until = net_seconds() + seconds;
    NetLobby l;
    for (;;) {
        net_get_lobby(&l);
        if (done(&l))
            return;
        if (l.state == NET_FAILED || net_seconds() > until) {
            printf("FAILED waiting for %s: state %d, %s %s\n", what, l.state, l.error_code, l.error);
            exit(1);
        }
        Sleep(20);
    }
}

static int players_wanted = 4;
static int in_lobby(const NetLobby *l) { return l->state == NET_LOBBY && l->code[0]; }
static int all_ready(const NetLobby *l)
{
    int i, ready = 0;
    for (i = 0; i < l->nmembers; i++)
        ready += l->member[i].ready;
    return l->nmembers >= players_wanted && ready == l->nmembers;
}
static int racing(const NetLobby *l) { return l->state == NET_RACING; }

enum { PAD_RIGHT = 0x0020, PAD_LEFT = 0x0080, PAD_CROSS = 0x4000 };  /* hle.h's libpad bits */
static NetInput bot_pad(int slot, uint32_t field)
{
    NetInput in = {PAD_CROSS, 128, 128, 128, 128};
    uint32_t t = (field + 37 * slot) % 300;   /* five seconds: steer one way, ride, the other way */
    if (t < 40) in.buttons |= PAD_LEFT, in.lx = 0;
    else if (t >= 150 && t < 190) in.buttons |= PAD_RIGHT, in.lx = 255;
    return in;
}

/* --replay FILE (with --bot): a player of a state-sync race (port/hle/online.c, finding 047)
   without a game. It says it has loaded and what it rides, takes the race's GO from the first
   game's GO event, then sends the snapshots a game recorded (DHD_NET_RECORD) as its own rider when
   the race clock reaches each one's, and its finish when they run out. */
enum { EVENT_VERSION = 2, EVENT_LOADED = 2, EVENT_SETUP = 3, EVENT_GO = 5, EVENT_FINISH = 6, EVENT_RESULTS = 7 };

static void put32(uint8_t *o, int32_t v) { memcpy(o, &v, 4); }

static int replay(const char *path, int slot, int rider, double seconds)
{
    static uint8_t data[1 << 22];
    FILE *fp = fopen(path, "rb");
    size_t size = fp ? fread(data, 1, sizeof data, fp) : 0, at = 0;
    uint8_t e[30] = {EVENT_VERSION, EVENT_SETUP}, frame[NET_FRAME_MAX];
    double go = 0, until = net_seconds() + seconds;
    int sent = 0, last_clock = 0, results = 0, n;
    if (fp) fclose(fp);
    if (!size) {
        printf("FAILED: no recording in %s\n", path);
        return 1;
    }
    e[1] = EVENT_LOADED;
    net_send_frame(NET_FRAME_EVENT, e, 2);
    e[1] = EVENT_SETUP;
    put32(e + 2, 0x20 * (rider + 1)), put32(e + 6, rider), put32(e + 18, 8), put32(e + 22, 3);
    net_send_frame(NET_FRAME_EVENT, e, sizeof e);
    while (net_seconds() < until && !results) {
        while ((n = net_receive_frame(frame, sizeof frame)) > 0)
            if (frame[0] == NET_FRAME_EVENT && n >= 4 && frame[2] == EVENT_VERSION) {
                if (frame[3] == EVENT_GO && n >= 12 && !go) {
                    memcpy(&go, frame + 4, 8);
                    printf("GO from player %d\n", frame[1]);
                } else if (frame[3] == EVENT_RESULTS) results = 1;
            }
        double clock = go ? (net_server_ms() - go) / (1000 / 59.94) : -1;
        while (go && at + 2 <= size) {
            uint16_t len;
            int32_t when;
            memcpy(&len, data + at, 2);
            if (at + 2 + len > size || len < 6) { at = size; break; }
            memcpy(&when, data + at + 4, 4);
            if (when > clock) break;
            data[at + 3] = (uint8_t)slot;  /* the rider is this player's */
            net_send_frame(NET_FRAME_STATE, data + at + 2, len);
            last_clock = when, sent++, at += 2 + len;
        }
        if (go && at + 2 > size && last_clock) {
            uint8_t f[7] = {EVENT_VERSION, EVENT_FINISH, (uint8_t)slot};
            put32(f + 3, last_clock);
            net_send_frame(NET_FRAME_EVENT, f, sizeof f);
            printf("FINISH at race tick %d after %d snapshots\n", last_clock, sent);
            last_clock = 0;
        }
        Sleep(5);
    }
    printf("RESULT {\"slot\":%d,\"snapshots\":%d,\"results\":%d}\n", slot, sent, results);
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

int main(int argc, char **argv)
{
    const char *server = NULL, *name = "Tester", *mode = NULL, *code = NULL, *code_file = NULL;
    const char *version = "dhd-port nettool 1", *iso = "0000000000000000000000000000000000000000000000000000000000000000";
    const char *replay_file = NULL;
    int bot = 0, rider = 4, seconds = 600;
    unsigned fields = 600, drop_at = 0, corrupt_from = 0, f;
    int i, slot, ai_from[NET_MAX_SLOTS], late_fields = 0;
    double t0, waited = 0, worst_wait = 0;
    uint64_t state;
    uint32_t desync_field = 0, desync_slots = 0, waited_for = 0;
    NetLobby l;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--server") && i + 1 < argc) server = argv[++i];
        else if (!strcmp(argv[i], "--name") && i + 1 < argc) name = argv[++i];
        else if (!strcmp(argv[i], "--fields") && i + 1 < argc) fields = (unsigned)atoi(argv[++i]);
        else if (!strcmp(argv[i], "--drop-at") && i + 1 < argc) drop_at = (unsigned)atoi(argv[++i]);
        else if (!strcmp(argv[i], "--corrupt-from") && i + 1 < argc) corrupt_from = (unsigned)atoi(argv[++i]);
        else if (!strcmp(argv[i], "--players") && i + 1 < argc) players_wanted = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--code-file") && i + 1 < argc) code_file = argv[++i];
        else if (!strcmp(argv[i], "--version") && i + 1 < argc) version = argv[++i];
        else if (!strcmp(argv[i], "--iso") && i + 1 < argc) iso = argv[++i];
        else if (!strcmp(argv[i], "--bot")) bot = 1;
        else if (!strcmp(argv[i], "--replay") && i + 1 < argc) replay_file = argv[++i];
        else if (!strcmp(argv[i], "--rider") && i + 1 < argc) rider = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--seconds") && i + 1 < argc) seconds = atoi(argv[++i]);
        else if (!mode) mode = argv[i];
        else code = argv[i];
    }
    if (!mode || (!strcmp(mode, "join") && !code)) {
        printf("usage: nettool [--server URL] [--name NAME] [--fields N] [--drop-at F] [--corrupt-from F]\n"
               "               [--bot --version V --iso SHA256] host [--players N] [--code-file PATH] | join CODE | list\n");
        return 2;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    timeBeginPeriod(1);
    net_init(server, name, version, iso);
    if (!strcmp(mode, "list")) {
        NetLobbyInfo info[32];
        int n = net_list_lobbies(info, 32);
        printf("%d public lobbies\n", n);
        for (i = 0; i < n; i++)
            printf("  %s  %s  %d/%d  %s\n", info[i].code, info[i].room_name, info[i].players, info[i].max_players,
                   info[i].version);
        return n < 0;
    }
    if (!strcmp(mode, "host") ? net_create_room() : net_join_room(code)) {
        printf("FAILED: bad code or no server (set DHD_NET_SERVER)\n");
        return 1;
    }
    wait_for(in_lobby, 15, "the lobby");
    net_get_lobby(&l);
    printf("CODE %s member %d%s\n", l.code, l.member_id, l.is_host ? " (host)" : "");
    if (code_file) {
        FILE *fp = fopen(code_file, "w");
        fprintf(fp, "%s\n", l.code);
        fclose(fp);
    }
    net_pick(-1, replay_file ? rider + 1 : l.member_id % 8, 1);
    net_set_ready(1);
    if (l.is_host) {
        wait_for(all_ready, 60, "the players");
        Sleep(300);
        net_start_race();
    }
    wait_for(racing, 60, "the start");
    net_get_lobby(&l);
    slot = l.local_slot;
    printf("RACE slot %d of %d, input delay %d, ping %d ms, seed %u\n", slot, l.nslots, l.input_delay, l.ping_ms,
           l.race_seed);
    if (replay_file) {
        int r = replay(replay_file, slot, rider, seconds);
        net_leave();
        return r;
    }
    while (net_seconds() < l.start_at && net_seconds() > l.start_at - 10)
        Sleep(1);
    state = l.race_seed;
    for (i = 0; i < NET_MAX_SLOTS; i++)
        ai_from[i] = -1;
    t0 = net_seconds();
    for (f = 0; f < fields; f++) {
        NetPad out[NET_MAX_SLOTS];
        NetInput in = bot ? bot_pad(slot, f) : pad_of(slot, f);
        double due = t0 + f / 59.94, start;
        int r, s;
        while (net_seconds() < due)
            Sleep(1);
        if (net_seconds() > due + 0.05)
            late_fields++;
        if (drop_at && f == drop_at) {
            printf("DROPPED at field %u\n", f);
            TerminateProcess(GetCurrentProcess(), 3);  /* the OS closes the socket, as a crash would */
        }
        net_submit_local(f, &in);
        start = net_seconds();
        while ((r = net_inputs_for_field(f, out, 50)) == NET_WAIT && net_seconds() < start + (bot ? 180 : 15))
            waited_for |= net_waiting_for();
        if (r != NET_OK) {
            printf("FAILED at field %u (%d), waiting for %x\n", f, r, net_waiting_for());
            return 1;
        }
        waited += net_seconds() - start;
        if (net_seconds() - start > worst_wait)
            worst_wait = net_seconds() - start;
        if (net_seconds() > due + 0.002)
            t0 += net_seconds() - due;  /* slip, don't catch up: an early PC falls in step after one wait */
        for (s = 0; s < l.nslots; s++) {
            state = net_hash64(&out[s], sizeof out[s], state);
            if (out[s].kind == NET_SLOT_AI && ai_from[s] < 0)
                ai_from[s] = (int)f;
        }
        if (net_hash_due(f) && !bot)
            net_submit_hash(f, state ^ (corrupt_from && f >= corrupt_from));
    }
    Sleep(1500);  /* the last hash comparisons */
    net_get_lobby(&l);
    printf("RESULT {\"slot\":%d,\"fields\":%u,\"state\":\"%016llx\",\"delay\":%d,\"ping\":%d,\"waitedMs\":%.0f,"
           "\"worstWaitMs\":%.0f,\"lateFields\":%d,\"waitedFor\":%u,\"offline\":%d,\"aiFrom\":[",
           slot, f, (unsigned long long)state, l.input_delay, l.ping_ms, waited * 1000, worst_wait * 1000, late_fields,
           waited_for, l.offline);
    for (i = 0; i < l.nslots; i++)
        printf("%s%d", i ? "," : "", ai_from[i]);
    printf("],\"desync\":");
    if (net_desync(&desync_field, &desync_slots))
        printf("{\"field\":%u,\"slots\":%u}}\n", desync_field, desync_slots);
    else
        printf("null}\n");
    net_leave();
    timeEndPeriod(1);
    return 0;
}
