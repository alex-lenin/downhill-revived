/* One online player: the lobby state machine and the race around the lockstep core, driven by
   messages from any transport (ws_winhttp.c, or test_loopback.c's fake relay). Thread-safe: the
   network threads deliver messages while the game thread waits for inputs. */
#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <windows.h>

#include "lockstep.h"
#include "net.h"

enum { NET_PROTOCOL = 1 };

typedef struct {
    void *ctx;
    void (*send_text)(void *ctx, const char *text);
    void (*send_binary)(void *ctx, const void *data, size_t n);
} NetTransport;

typedef struct {
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE changed;
    NetTransport tr;
    int connected, leaving;
    char name[100], version[64], iso[65];
    NetLobby lobby;
    Lockstep ls;
    int racing;
    /* what to repeat after a lobby reconnect */
    int picked, rider, bike, ready;
    /* pings and the server clock */
    double next_ping, last_rx, clock_offset;  /* offset: server ms - local ms */
    int have_offset;
    float rtt[5];
    int nrtt;
    /* the race */
    uint32_t waiting;
    double stall_since, last_resend;
    int desynced;
    uint32_t desync_field, desync_slots;
    uint64_t sent_hash[LS_HASHES];
    uint32_t sent_hash_field[LS_HASHES];
    /* STATE and EVENT frames received, oldest first (a full queue drops the oldest) */
    uint8_t relayed[NET_RELAY_QUEUE][NET_FRAME_MAX];
    uint16_t relayed_size[NET_RELAY_QUEUE];
    unsigned relayed_head, relayed_count;
} NetClient;

double net_now_ms(void);  /* a monotonic clock */

void client_init(NetClient *c, const char *name, const char *version, const char *iso);
void client_free(NetClient *c);
/* The transport connected: hello goes out. */
void client_connected(NetClient *c, const NetTransport *tr);
void client_on_text(NetClient *c, const char *text, size_t n);
void client_on_binary(NetClient *c, const uint8_t *data, size_t n);
/* The connection ended. Returns 1 when it should rejoin the room by its code (a lobby drop). */
int client_on_closed(NetClient *c, int code, const char *reason);
/* Every few ms from a network thread: pings and resends while stalled. Returns 1 when the
   server has been silent too long and the connection should be dropped. */
int client_tick(NetClient *c);

void client_set_room(NetClient *c, const NetRoomSettings *s);
void client_pick(NetClient *c, int member, int rider, int bike);
void client_ready(NetClient *c, int ready);
void client_kick(NetClient *c, int member);
void client_start(NetClient *c);
void client_end(NetClient *c);
void client_leave(NetClient *c);  /* says so before the transport closes */
void client_lobby(NetClient *c, NetLobby *out);

void client_submit_local(NetClient *c, uint32_t field, const NetInput *in);
int client_inputs_for_field(NetClient *c, uint32_t field, NetPad out[NET_MAX_SLOTS], unsigned timeout_ms);
uint32_t client_waiting(NetClient *c);
void client_submit_hash(NetClient *c, uint32_t field, uint64_t hash);
int client_send_frame(NetClient *c, int type, const void *payload, size_t n);
int client_receive_frame(NetClient *c, uint8_t *out, size_t max);
int client_desync(NetClient *c, uint32_t *field, uint32_t *slots);

#endif
