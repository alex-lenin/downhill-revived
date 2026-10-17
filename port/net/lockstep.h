/* The lockstep core: which pad input every slot has for every field, independent of the
   transport so the tests can drive it directly. Not thread-safe; client.c locks around it.

   Fields are the game's vsyncs (59.94/s), numbered from 0 at the race start. Each PC owns one
   slot. Its pad, sampled at field F, is scheduled for field F + delay and sent in INPUT frames
   that repeat the last LS_REDUNDANCY fields, so a lost or reordered frame costs nothing. A field
   can be played once every slot has its input (or the slot is empty or driven by the AI).
   Fields 0 .. delay-1 are neutral for everyone and never sent.

   Wire format: docs/online-protocol.md (version 1), little-endian.
     INPUT {u8 type=1, u8 slot, u32 first field, u8 count 1..16, count x {u16 buttons, u8 lx, ly, rx, ry}}
           buttons active low on the wire (0xFFFF = none held), active high in NetInput
     HASH  {u8 type=2, u8 slot, u32 field, u64 hash}
     PING / PONG {u8 type=3 / 4, u8 slot, payload <= 32 bytes}  (client.c) */
#ifndef NET_LOCKSTEP_H
#define NET_LOCKSTEP_H

#include <stddef.h>
#include <stdint.h>

#include "net.h"

enum { LS_WINDOW = 512,      /* fields held per slot: received ahead, and played behind */
       LS_REDUNDANCY = 3,    /* fields per regular INPUT frame */
       LS_MAX_DELAY = 60,
       LS_HASHES = 16 };     /* hash checkpoints remembered */
enum { LS_MSG_INPUT = 1, LS_MSG_HASH = 2, LS_MSG_PING = 3, LS_MSG_PONG = 4 };
enum { LS_INPUT_HEADER = 7, LS_INPUT_ENTRY = 6, LS_HASH_SIZE = 14, LS_MAX_COUNT = 16,
       LS_MAX_FRAME = LS_INPUT_HEADER + LS_MAX_COUNT * LS_INPUT_ENTRY };

typedef struct {
    uint8_t kind;              /* NET_SLOT_* */
    uint32_t ai_from;          /* a human slot is the AI's from this field on */
    int64_t last_received;     /* the highest field any of its frames named, -1 none */
    uint32_t field[LS_WINDOW]; /* the field each ring entry holds, UINT32_MAX for none */
    NetInput in[LS_WINDOW];
} LsSlot;

typedef struct {
    uint32_t field;
    uint8_t have;              /* slots whose hash is here */
    uint64_t hash[NET_MAX_SLOTS];
} LsHash;

typedef struct {
    int nslots, local, delay;
    int started;
    uint32_t first_sent;       /* the start's delay: fields below it are never sent */
    uint32_t next;             /* the lowest field not played yet */
    int64_t local_last;        /* the last field the local input is scheduled for, -1 none */
    LsSlot slot[NET_MAX_SLOTS];
    LsHash hashes[LS_HASHES];
    int desynced;              /* the first mismatch found */
    uint32_t desync_field, desync_slots;
    struct { unsigned received, duplicate, old, too_far, ignored, bad; } stats;
} Lockstep;

/* What ls_receive found in a frame. */
typedef struct {
    int type;                  /* LS_MSG_*, or -1 for a malformed frame */
    int slot;
    uint32_t field;
    int count;                 /* INPUT: entries; new ones are counted in fresh */
    int fresh;
} LsEvent;

void ls_init(Lockstep *ls, int nslots, int local_slot, int delay);
void ls_set_slot(Lockstep *ls, int slot, int kind);   /* before the start */
/* Changes the delay; also allowed during the race (each sample then drops or repeats). */
int ls_set_delay(Lockstep *ls, int delay);
/* Starts field 0: the first delay fields are neutral for every slot. */
void ls_start(Lockstep *ls);

/* The local pad sampled at field. Returns how many fields it filled: 1 normally, 0 when the
   delay shrank (the sample is dropped), more when it grew (the gap repeats this input), -1 when
   it is too far ahead of the played fields. */
int ls_submit_local(Lockstep *ls, uint32_t field, const NetInput *in);
/* An INPUT frame of the local slot for count fields from first (only fields it has). */
size_t ls_encode_input(const Lockstep *ls, uint32_t first, int count, uint8_t *buf, size_t cap);
/* The regular frame: the last LS_REDUNDANCY scheduled fields. */
size_t ls_encode_recent(const Lockstep *ls, uint8_t *buf, size_t cap);
/* While stalled: the oldest local fields another player may still lack. A player whose input
   reached field G sampled it at G - delay, after playing every field before that, so it held
   ours up to G - delay - 1. */
size_t ls_encode_resend(const Lockstep *ls, uint8_t *buf, size_t cap);
size_t ls_encode_hash(int slot, uint32_t field, uint64_t hash, uint8_t *buf);

/* A binary frame from the relay. */
LsEvent ls_receive(Lockstep *ls, const uint8_t *msg, size_t n);
/* The server announced the slot's player left: the AI drives it from field on. */
void ls_slot_ai_from(Lockstep *ls, int slot, uint32_t field);
/* One past the highest field received from the slot (0 when none): where the AI takes over
   when the server's left message has no fromField. Every PC got the same relayed frames, so
   every PC finds the same field. */
uint32_t ls_after_last_received(const Lockstep *ls, int slot);

/* Slots still missing field's input (0 when the field can be played). */
uint32_t ls_missing(const Lockstep *ls, uint32_t field);
/* The inputs of a complete field; returns 0, or -1 when it is incomplete or no longer held. */
int ls_take(Lockstep *ls, uint32_t field, NetPad out[NET_MAX_SLOTS]);
/* The latest field a slot has contiguously from next (next - 1 when it has none). */
int64_t ls_have_through(const Lockstep *ls, int slot);

/* Hash checkpoints: the local one and the others' ones; a mismatch sets desynced. */
void ls_local_hash(Lockstep *ls, uint32_t field, uint64_t hash);

/* The automatic input delay for a relay, as the server computes it (docs/online-protocol.md):
   a frame from one player reaches another after half of each one's round trip, so the delay
   covers half the two largest round trips (one player: half its own) plus a field of slack,
   1..8 fields. rtt_ms holds each player's ping to the relay; negative (unknown) counts as 0. */
int ls_auto_delay(const float *rtt_ms, int n);

#endif
