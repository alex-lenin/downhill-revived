/* The lockstep core (lockstep.h). */
#include "lockstep.h"

#include <math.h>
#include <string.h>

#define NONE 0xFFFFFFFFu

static const NetInput neutral = {0, 0x80, 0x80, 0x80, 0x80};

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v, p[1] = (uint8_t)(v >> 8), p[2] = (uint8_t)(v >> 16), p[3] = (uint8_t)(v >> 24);
}

static uint32_t get32(const uint8_t *p)
{
    return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24;
}

void ls_init(Lockstep *ls, int nslots, int local_slot, int delay)
{
    int s, i;
    memset(ls, 0, sizeof *ls);
    ls->nslots = nslots < 1 ? 1 : nslots > NET_MAX_SLOTS ? NET_MAX_SLOTS : nslots;
    ls->local = local_slot;
    ls->local_last = -1;
    ls_set_delay(ls, delay);
    for (s = 0; s < NET_MAX_SLOTS; s++) {
        ls->slot[s].kind = s < ls->nslots ? NET_SLOT_HUMAN : NET_SLOT_EMPTY;
        ls->slot[s].ai_from = NONE;
        ls->slot[s].last_received = -1;
        for (i = 0; i < LS_WINDOW; i++)
            ls->slot[s].field[i] = NONE;
    }
    for (i = 0; i < LS_HASHES; i++)
        ls->hashes[i].field = NONE;
}

void ls_set_slot(Lockstep *ls, int slot, int kind)
{
    if (slot >= 0 && slot < ls->nslots)
        ls->slot[slot].kind = (uint8_t)kind;
}

int ls_set_delay(Lockstep *ls, int delay)
{
    ls->delay = delay < 0 ? 0 : delay > LS_MAX_DELAY ? LS_MAX_DELAY : delay;
    return ls->delay;
}

static void schedule(Lockstep *ls, uint32_t field, const NetInput *in)
{
    LsSlot *s = &ls->slot[ls->local];
    s->field[field % LS_WINDOW] = field;
    s->in[field % LS_WINDOW] = *in;
    ls->local_last = field;
}

void ls_start(Lockstep *ls)
{
    int s, f;
    ls->started = 1;
    ls->first_sent = (uint32_t)ls->delay;
    for (s = 0; s < ls->nslots; s++)
        for (f = 0; f < ls->delay; f++) {
            ls->slot[s].field[f] = (uint32_t)f;
            ls->slot[s].in[f] = neutral;
        }
    ls->local_last = ls->delay - 1;
}

int ls_submit_local(Lockstep *ls, uint32_t field, const NetInput *in)
{
    int64_t target = (int64_t)field + ls->delay, f;
    int n = 0;
    if (target <= ls->local_last)
        return 0;
    if (target >= (int64_t)ls->next + LS_WINDOW)
        return -1;
    for (f = ls->local_last + 1; f <= target; f++, n++)
        schedule(ls, (uint32_t)f, in);
    return n;
}

size_t ls_encode_input(const Lockstep *ls, uint32_t first, int count, uint8_t *buf, size_t cap)
{
    const LsSlot *s = &ls->slot[ls->local];
    uint8_t *p = buf + LS_INPUT_HEADER;
    int n = 0;
    if (first < ls->first_sent)
        count -= (int)(ls->first_sent - first), first = ls->first_sent;
    if (count > LS_MAX_COUNT)
        count = LS_MAX_COUNT;
    if (count <= 0 || ls->local_last < 0 || (int64_t)first > ls->local_last || cap < LS_INPUT_HEADER)
        return 0;
    if ((int64_t)first + count - 1 > ls->local_last)
        count = (int)(ls->local_last - first + 1);
    /* only the fields still held, contiguous from first */
    while (n < count && s->field[(first + n) % LS_WINDOW] == first + (uint32_t)n &&
           (size_t)(p - buf) + LS_INPUT_ENTRY <= cap) {
        const NetInput *in = &s->in[(first + n) % LS_WINDOW];
        p[0] = (uint8_t)~in->buttons, p[1] = (uint8_t)(~in->buttons >> 8);
        p[2] = in->lx, p[3] = in->ly, p[4] = in->rx, p[5] = in->ry;
        p += LS_INPUT_ENTRY, n++;
    }
    if (!n)
        return 0;
    buf[0] = LS_MSG_INPUT;
    buf[1] = (uint8_t)ls->local;
    put32(buf + 2, first);
    buf[6] = (uint8_t)n;
    return (size_t)(p - buf);
}

size_t ls_encode_recent(const Lockstep *ls, uint8_t *buf, size_t cap)
{
    int64_t first = ls->local_last - LS_REDUNDANCY + 1;
    if (ls->local_last < 0)
        return 0;
    return ls_encode_input(ls, (uint32_t)(first < 0 ? 0 : first), LS_REDUNDANCY, buf, cap);
}

size_t ls_encode_resend(const Lockstep *ls, uint8_t *buf, size_t cap)
{
    int64_t first = INT64_MAX;
    int s;
    for (s = 0; s < ls->nslots; s++)
        if (s != ls->local && ls->slot[s].kind == NET_SLOT_HUMAN && ls->slot[s].ai_from == NONE) {
            int64_t held = ls_have_through(ls, s) - ls->delay;  /* it held ours before this */
            if (held < first)
                first = held;
        }
    if (first == INT64_MAX)
        return 0;
    return ls_encode_input(ls, (uint32_t)(first < 0 ? 0 : first), LS_MAX_COUNT, buf, cap);
}

size_t ls_encode_hash(int slot, uint32_t field, uint64_t hash, uint8_t *buf)
{
    buf[0] = LS_MSG_HASH;
    buf[1] = (uint8_t)slot;
    put32(buf + 2, field);
    put32(buf + 6, (uint32_t)hash);
    put32(buf + 10, (uint32_t)(hash >> 32));
    return LS_HASH_SIZE;
}

static void hash_seen(Lockstep *ls, int slot, uint32_t field, uint64_t hash)
{
    LsHash *h = &ls->hashes[field / NET_HASH_PERIOD % LS_HASHES];
    int s;
    if (h->field != NONE && h->field > field)
        return;  /* older than what the entry holds now */
    if (h->field != field) {
        memset(h, 0, sizeof *h);
        h->field = field;
    }
    h->hash[slot] = hash;
    h->have |= 1u << slot;
    if (!(h->have & 1u << ls->local))
        return;
    for (s = 0; s < ls->nslots; s++)
        if (s != ls->local && h->have & 1u << s && h->hash[s] != h->hash[ls->local]) {
            if (!ls->desynced)
                ls->desynced = 1, ls->desync_field = field;
            if (ls->desync_field == field)
                ls->desync_slots |= 1u << s;
        }
}

void ls_local_hash(Lockstep *ls, uint32_t field, uint64_t hash)
{
    hash_seen(ls, ls->local, field, hash);
}

LsEvent ls_receive(Lockstep *ls, const uint8_t *msg, size_t n)
{
    LsEvent ev = {-1, 0, 0, 0, 0};
    if (n < 2 || msg[1] >= ls->nslots) {
        ls->stats.bad++;
        return ev;
    }
    ev.slot = msg[1];
    switch (msg[0]) {
    case LS_MSG_INPUT: {
        LsSlot *s = &ls->slot[ev.slot];
        int i;
        if (n < LS_INPUT_HEADER || !msg[6] || msg[6] > LS_MAX_COUNT ||
            n != LS_INPUT_HEADER + (size_t)msg[6] * LS_INPUT_ENTRY)
            break;
        ev.type = LS_MSG_INPUT;
        ev.field = get32(msg + 2);
        ev.count = msg[6];
        ls->stats.received++;
        if (ev.slot == ls->local || s->kind != NET_SLOT_HUMAN) {
            ls->stats.ignored++;  /* our own echo, or nobody plays that slot */
            return ev;
        }
        for (i = 0; i < ev.count; i++) {
            const uint8_t *p = msg + LS_INPUT_HEADER + i * LS_INPUT_ENTRY;
            uint32_t f = ev.field + (uint32_t)i, at = f % LS_WINDOW;
            if (f >= ev.field && (int64_t)f > s->last_received)
                s->last_received = f;
            if (f < ev.field || f >= s->ai_from)
                break;  /* wrapped, or the AI drives it by now */
            if (f < ls->next)
                ls->stats.old++;
            else if (f - ls->next >= LS_WINDOW)
                ls->stats.too_far++;
            else if (s->field[at] == f)
                ls->stats.duplicate++;
            else {
                s->field[at] = f;
                s->in[at].buttons = (uint16_t)~(p[0] | p[1] << 8);
                s->in[at].lx = p[2], s->in[at].ly = p[3], s->in[at].rx = p[4], s->in[at].ry = p[5];
                ev.fresh++;
            }
        }
        return ev;
    }
    case LS_MSG_HASH:
        if (n != LS_HASH_SIZE)
            break;
        ev.type = LS_MSG_HASH;
        ev.field = get32(msg + 2);
        if (ev.slot != ls->local)
            hash_seen(ls, ev.slot, ev.field, get32(msg + 6) | (uint64_t)get32(msg + 10) << 32);
        return ev;
    }
    ev.type = -1;
    ls->stats.bad++;
    return ev;
}

void ls_slot_ai_from(Lockstep *ls, int slot, uint32_t field)
{
    if (slot >= 0 && slot < ls->nslots && field < ls->slot[slot].ai_from)
        ls->slot[slot].ai_from = field;
}

uint32_t ls_after_last_received(const Lockstep *ls, int slot)
{
    return slot >= 0 && slot < ls->nslots ? (uint32_t)(ls->slot[slot].last_received + 1) : 0;
}

static int has(const Lockstep *ls, int slot, uint32_t field)
{
    const LsSlot *s = &ls->slot[slot];
    return s->kind != NET_SLOT_HUMAN || field >= s->ai_from || s->field[field % LS_WINDOW] == field;
}

uint32_t ls_missing(const Lockstep *ls, uint32_t field)
{
    uint32_t mask = 0;
    int s;
    for (s = 0; s < ls->nslots; s++)
        if (!has(ls, s, field))
            mask |= 1u << s;
    return mask;
}

int ls_take(Lockstep *ls, uint32_t field, NetPad out[NET_MAX_SLOTS])
{
    int s;
    if (ls_missing(ls, field))
        return -1;
    for (s = 0; s < NET_MAX_SLOTS; s++) {
        const LsSlot *slot = &ls->slot[s];
        out[s].in = neutral;
        if (s >= ls->nslots || slot->kind == NET_SLOT_EMPTY)
            out[s].kind = NET_SLOT_EMPTY;
        else if (slot->kind == NET_SLOT_AI || field >= slot->ai_from)
            out[s].kind = NET_SLOT_AI;
        else {
            out[s].kind = NET_SLOT_HUMAN;
            out[s].in = slot->in[field % LS_WINDOW];
        }
    }
    if (field >= ls->next)
        ls->next = field + 1;
    return 0;
}

int64_t ls_have_through(const Lockstep *ls, int slot)
{
    uint32_t f = ls->next;
    while (f - ls->next < LS_WINDOW && has(ls, slot, f))
        f++;
    return (int64_t)f - 1;
}

int ls_auto_delay(const float *rtt_ms, int n)
{
    double worst = 0, second = 0, one_way;
    int i, delay;
    for (i = 0; i < n; i++) {
        double r = rtt_ms[i] > 0 ? rtt_ms[i] : 0;
        if (r > worst)
            second = worst, worst = r;
        else if (r > second)
            second = r;
    }
    one_way = n == 1 ? worst / 2 : (worst + second) / 2;
    delay = (int)ceil(one_way / 16.68) + 1;  /* the server's NTSC field length */
    return delay < 1 ? 1 : delay > 8 ? 8 : delay;
}
