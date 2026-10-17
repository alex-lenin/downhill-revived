/* Online races: the API the rest of the port uses (docs/ONLINE-PLAN.md, finding 039, the wire
   protocol in docs/online-protocol.md).

   Every PC runs the same race; only pad inputs travel, through a relay over a WebSocket. A
   background thread keeps the connection (lobby JSON, binary race frames, pings); the game
   thread calls the race functions below once per field.

   Integration, per field F from the race start (pad_vsync, for the 4 multitap pads):
       net_submit_local(F, &pad_of_this_pc);          host_pad_read's state, as a NetInput
       while (net_inputs_for_field(F, pads, 50) == NET_WAIT)
           show "waiting for" net_waiting_for();      keep the window responsive
       slot i's pad = pads[i] (NET_SLOT_AI: the game's AI rider, NET_SLOT_EMPTY: no pad)
   and after field F ran, when net_hash_due(F):
       net_submit_hash(F, net_hash_ranges(ranges, n));
   Pressure-sensitive buttons don't travel: derive them from the buttons (pressed = 255) on
   every PC, so all of them see the same pads. */
#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <stdint.h>

enum { NET_MAX_SLOTS = 8, NET_HASH_PERIOD = 60 };  /* a room holds 8; the game's multitap 4 */
enum { NET_OK = 0, NET_WAIT = 1, NET_ERROR = -1 };
enum { NET_SLOT_EMPTY = 0, NET_SLOT_HUMAN = 1, NET_SLOT_AI = 2 };

/* One pad as it travels: libpad's button bits (1 = pressed, hle.h's PAD_*), sticks 0..255
   with 128 centred. */
typedef struct {
    uint16_t buttons;
    uint8_t lx, ly, rx, ry;
} NetInput;

typedef struct {
    NetInput in;           /* neutral unless kind is NET_SLOT_HUMAN */
    uint8_t kind;          /* NET_SLOT_* for this field */
} NetPad;

/* ---- connection and lobby ---- */

enum { NET_IDLE, NET_CONNECTING, NET_LOBBY, NET_RACING, NET_RECONNECTING, NET_FAILED };

typedef struct {
    int id;                    /* member id: fixed for the connection */
    char name[100];            /* UTF-8, up to 24 characters */
    int ready, rider, bike;
    int rtt_ms;                /* the server's measure, -1 unknown */
} NetMember;

typedef struct {
    int slot, member_id;
    char name[100];
    int rider, bike;
    int left;                  /* gone: the AI drives the rider from left_field on */
    uint32_t left_field;
} NetRacer;

typedef struct {
    int state;                 /* NET_IDLE... */
    unsigned seq;              /* bumps on every change */
    char code[8];              /* the join code */
    int member_id, host_id, is_host;
    char room_name[100];
    int is_public, max_players, started;
    int track, laps;
    long long seed;            /* -1 until set */
    int nmembers;
    NetMember member[NET_MAX_SLOTS];
    int input_delay;           /* the server's suggestion, final once the race starts */
    int ping_ms;               /* this PC to the relay, -1 unknown */
    /* the race, once started */
    int local_slot, nslots;
    NetRacer racer[NET_MAX_SLOTS];
    int race_track, race_laps;
    uint32_t race_seed;
    double start_at;           /* net_seconds() at which field 0 should run */
    int offline;               /* the connection dropped mid-race: everyone else is AI now */
    /* the last error */
    char error_code[32], error[200];
} NetLobby;

/* Host settings; fields at -1 (room_name NULL) are left as they are. */
typedef struct {
    int track, laps, is_public, max_players;
    long long seed;
    const char *room_name;
} NetRoomSettings;

typedef struct {
    char code[8], room_name[100], version[64], host_name[100];
    int track, players, max_players;
} NetLobbyInfo;

/* base_url: "wss://host" or "ws://127.0.0.1:8787". version: the build's version string (the
   server refuses players whose version differs from the host's). iso_sha256: 64 lowercase hex. */
void net_init(const char *base_url, const char *name, const char *version, const char *iso_sha256);
int net_create_room(void);
int net_join_room(const char *code);   /* -1 when the code can't be one */
void net_leave(void);
/* Public lobbies (blocking HTTP GET); returns how many, -1 on error. */
int net_list_lobbies(NetLobbyInfo *out, int max);
void net_set_room(const NetRoomSettings *s);       /* host */
void net_pick(int member, int rider, int bike);    /* member -1: this PC */
void net_set_ready(int ready);
void net_kick(int member);                         /* host */
void net_start_race(void);                         /* host; everyone must be ready */
void net_end_race(void);                           /* host: back to the lobby */
void net_get_lobby(NetLobby *out);                 /* a copy, for the UI */
double net_seconds(void);                          /* the clock start_at uses */
/* The server's clock (ms since the Unix epoch), estimated from the lobby pings' round-trips: the
   same moment on every PC to a few ms, for starting the race together. */
double net_server_ms(void);

/* ---- the race ---- */

int net_racing(void);
int net_local_slot(void);
/* The local pad, sampled at field (it plays at field + the input delay). */
void net_submit_local(uint32_t field, const NetInput *in);
/* Fills out[NET_MAX_SLOTS] with field's pads. Waits up to timeout_ms for the other players;
   NET_WAIT when someone is still missing (net_waiting_for says who), NET_ERROR when not racing. */
int net_inputs_for_field(uint32_t field, NetPad out[NET_MAX_SLOTS], unsigned timeout_ms);
uint32_t net_waiting_for(void);   /* slot mask */

/* ---- state sync (finding 044) ---- */

/* Binary frames the relay forwards as they are to every other racer: a snapshot of riders this
   PC owns, an event it decided for another PC's rider. A frame is type, slot (the server sets
   it), payload. */
enum { NET_FRAME_STATE = 5, NET_FRAME_EVENT = 6, NET_FRAME_MAX = 256, NET_RELAY_QUEUE = 128 };
/* Sends type and payload (at most NET_FRAME_MAX - 2 bytes); 0 when not racing. */
int net_send_frame(int type, const void *payload, size_t n);
/* The oldest frame received, whole (out[0] type, out[1] sender's slot); its size, 0 if none. */
int net_receive_frame(uint8_t *out, size_t max);

/* ---- desync detection ---- */

typedef struct {
    const void *base;
    size_t size;
} NetRange;

/* A 64-bit hash (XXH64 per range, chained through the seed) of memory ranges. */
uint64_t net_hash_ranges(const NetRange *ranges, int n);
uint64_t net_hash64(const void *data, size_t size, uint64_t seed);  /* XXH64 */
/* The protocol hashes the state after fields 59, 119, ... */
static inline int net_hash_due(uint32_t field) { return field % NET_HASH_PERIOD == NET_HASH_PERIOD - 1; }
/* The local state hash after field ran (only net_hash_due fields are sent). */
void net_submit_hash(uint32_t field, uint64_t hash);
/* 1 once the server found differing hashes: the first field and the slots that differ from
   this PC (or all slots it named, when it had no hash from us). */
int net_desync(uint32_t *field, uint32_t *slots);

#endif
