/* A command-line player for testing the relay: it joins a room through net.h as the game does,
   and in the race replays a recorded rider (--replay, port/tools/online_race.py's bots) or just
   stays in it for --seconds, counting the others' frames; then it prints a RESULT line and leaves.

   usage: nettool [--server URL] [--name NAME] [--seconds S] [--version V --iso SHA256]
                  [--replay FILE --rider N] [--pick N] [--ready-after S]
                  host [--players N] [--code-file PATH] | join CODE | list
   The server is --server or DHD_NET_SERVER (ws://127.0.0.1:8787 for wrangler dev). With the
   game's --version and --iso hash it can join a room of real games (finding 041). */
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

/* --replay FILE (with --bot): a player of a state-sync race (port/hle/online.c, finding 047)
   without a game. It says it has loaded and what it rides, takes the race's GO from the first
   game's GO event, then sends the snapshots a game recorded (DHD_NET_RECORD) as its own rider when
   the race time reaches each one's (ms, finding 114), and its finish when they run out. */
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
        double clock = go ? net_server_ms() - go : -1;  /* race time, ms */
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
            put32(f + 3, (int32_t)(last_clock * 0.06));  /* race ticks (1/60 s fields) */
            net_send_frame(NET_FRAME_EVENT, f, sizeof f);
            printf("FINISH at race time %d ms after %d snapshots\n", last_clock, sent);
            last_clock = 0;
        }
        Sleep(5);
    }
    printf("RESULT {\"slot\":%d,\"snapshots\":%d,\"results\":%d}\n", slot, sent, results);
    return 0;
}

int main(int argc, char **argv)
{
    const char *server = NULL, *name = "Tester", *mode = NULL, *code = NULL, *code_file = NULL;
    const char *version = "dhd-port nettool 1", *iso = "0000000000000000000000000000000000000000000000000000000000000000";
    const char *replay_file = NULL;
    int rider = 4, seconds = 600, received = 0;
    int pick = -1;            /* --pick N: the lobby's rider number (1..16); 0 stays choosing */
    double ready_after = 0;   /* --ready-after S: Ready after S seconds; below 0 never */
    int i, slot;
    uint8_t frame[NET_FRAME_MAX];
    double until;
    NetLobby l;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--server") && i + 1 < argc) server = argv[++i];
        else if (!strcmp(argv[i], "--name") && i + 1 < argc) name = argv[++i];
        else if (!strcmp(argv[i], "--players") && i + 1 < argc) players_wanted = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--code-file") && i + 1 < argc) code_file = argv[++i];
        else if (!strcmp(argv[i], "--version") && i + 1 < argc) version = argv[++i];
        else if (!strcmp(argv[i], "--iso") && i + 1 < argc) iso = argv[++i];
        else if (!strcmp(argv[i], "--bot")) ;  /* (older scripts' flag; nothing to do) */
        else if (!strcmp(argv[i], "--replay") && i + 1 < argc) replay_file = argv[++i];
        else if (!strcmp(argv[i], "--rider") && i + 1 < argc) rider = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--seconds") && i + 1 < argc) seconds = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--pick") && i + 1 < argc) pick = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--ready-after") && i + 1 < argc) ready_after = atof(argv[++i]);
        else if (!mode) mode = argv[i];
        else code = argv[i];
    }
    if (!mode || (!strcmp(mode, "join") && !code)) {
        printf("usage: nettool [--server URL] [--name NAME] [--seconds S] [--version V --iso SHA256]\n"
               "               [--replay FILE --rider N] [--pick N] [--ready-after S] host [--players N] [--code-file PATH] | join CODE | list\n");
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
    if (pick == 0) net_pick(-1, -1, 0);  /* on the rider screen, as far as the room knows */
    else net_pick(-1, pick > 0 ? pick : replay_file ? rider + 1 : l.member_id % 8, 1);
    if (ready_after > 0) Sleep((DWORD)(ready_after * 1000));
    if (ready_after >= 0 && pick != 0) net_set_ready(1);
    if (l.is_host) {
        wait_for(all_ready, 60, "the players");
        Sleep(300);
        net_start_race();
    }
    wait_for(racing, seconds, "the start");
    net_get_lobby(&l);
    slot = l.local_slot;
    printf("RACE slot %d of %d, ping %d ms, seed %u\n", slot, l.nslots, l.ping_ms, l.race_seed);
    if (replay_file) {
        int r = replay(replay_file, slot, rider, seconds);
        net_leave();
        return r;
    }
    until = net_seconds() + seconds;
    while (net_seconds() < until && net_racing()) {
        while (net_receive_frame(frame, sizeof frame) > 0)
            received++;
        Sleep(5);
    }
    net_get_lobby(&l);
    printf("RESULT {\"slot\":%d,\"received\":%d,\"ping\":%d,\"offline\":%d}\n", slot, received, l.ping_ms,
           l.offline);
    net_leave();
    timeEndPeriod(1);
    return 0;
}
