/* Unit tests of the lockstep core (lockstep.c) and the state hash (hash.c). */
#include "lockstep.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures;
#define CHECK(c) do { if (!(c)) { printf("%s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #c); failures++; } } while (0)

static NetInput pad_of(int slot, uint32_t field)  /* a different pad for each slot and field */
{
    NetInput in;
    uint32_t x = (field + 1) * 2654435761u ^ (uint32_t)(slot + 1) * 40503u;
    in.buttons = (uint16_t)x, in.lx = (uint8_t)(x >> 16), in.ly = (uint8_t)(x >> 24);
    in.rx = (uint8_t)(x >> 8), in.ry = (uint8_t)slot;
    return in;
}

static int same(const NetInput *a, const NetInput *b)
{
    return a->buttons == b->buttons && a->lx == b->lx && a->ly == b->ly && a->rx == b->rx && a->ry == b->ry;
}

static void test_hash(void)
{
    static const char text[] = "Nobody inspects the spammish repetition";
    uint8_t big[1000];
    NetRange r[2];
    int i;
    CHECK(net_hash64("", 0, 0) == 0xEF46DB3751D8E999ull);
    CHECK(net_hash64("abc", 3, 0) == 0x44BC2CF5AD770999ull);
    CHECK(net_hash64(text, sizeof text - 1, 0) == 0xFBCEA83C8A378BF1ull);
    for (i = 0; i < (int)sizeof big; i++)
        big[i] = (uint8_t)(i * 7);
    r[0].base = big, r[0].size = 300, r[1].base = big + 300, r[1].size = 700;
    CHECK(net_hash_ranges(r, 2) == net_hash64(big + 300, 700, net_hash64(big, 300, 0)));
    CHECK(net_hash_ranges(r, 2) != net_hash64(big, sizeof big, 0));
    {
        uint64_t before = net_hash_ranges(r, 2);
        big[500] ^= 0x80;
        CHECK(net_hash_ranges(r, 2) != before);
    }
}

/* Two peers, a clean link: the inputs come out where they were scheduled. */
static void test_basic(void)
{
    static Lockstep a, b;
    uint8_t buf[512];
    NetPad out[NET_MAX_SLOTS];
    uint32_t f;
    ls_init(&a, 2, 0, 2), ls_init(&b, 2, 1, 2);
    ls_start(&a), ls_start(&b);
    CHECK(a.local_last == 1);
    CHECK(ls_missing(&a, 1) == 0 && ls_missing(&a, 2) == 3);  /* 0 and 1 are neutral */
    for (f = 0; f < 100; f++) {
        NetInput pa = pad_of(0, f), pb = pad_of(1, f);
        size_t n;
        CHECK(ls_submit_local(&a, f, &pa) == 1);
        CHECK(ls_submit_local(&b, f, &pb) == 1);
        n = ls_encode_recent(&a, buf, sizeof buf);
        CHECK(ls_receive(&b, buf, n).type == LS_MSG_INPUT);
        n = ls_encode_recent(&b, buf, sizeof buf);
        CHECK(ls_receive(&a, buf, n).type == LS_MSG_INPUT);
        CHECK(ls_missing(&a, f) == 0 && ls_missing(&b, f) == 0);
        CHECK(ls_take(&a, f, out) == 0);
        CHECK(out[0].kind == NET_SLOT_HUMAN && out[1].kind == NET_SLOT_HUMAN && out[2].kind == NET_SLOT_EMPTY);
        if (f >= 2) {
            NetInput e0 = pad_of(0, f - 2), e1 = pad_of(1, f - 2);
            CHECK(same(&out[0].in, &e0) && same(&out[1].in, &e1));
        } else
            CHECK(out[0].in.buttons == 0 && out[0].in.lx == 0x80);
        CHECK(ls_take(&b, f, out) == 0);
    }
    CHECK(a.stats.duplicate > 0);  /* the redundancy repeats every field three times */
    CHECK(ls_have_through(&a, 1) == 101);
}

/* Duplicates, reordering, malformed frames and the window's edges. */
static void test_receive_edges(void)
{
    static Lockstep a, b;
    uint8_t frames[20][64];
    size_t len[20];
    NetPad out[NET_MAX_SLOTS];
    int i;
    ls_init(&a, 2, 0, 0), ls_init(&b, 2, 1, 0);
    ls_start(&a), ls_start(&b);
    for (i = 0; i < 20; i++) {
        NetInput in = pad_of(1, (uint32_t)i);
        ls_submit_local(&b, (uint32_t)i, &in);
        len[i] = ls_encode_input(&b, (uint32_t)i, 1, frames[i], sizeof frames[i]);
        CHECK(len[i] == LS_INPUT_HEADER + LS_INPUT_ENTRY);
    }
    for (i = 19; i >= 0; i--)  /* reversed */
        CHECK(ls_receive(&a, frames[i], len[i]).fresh == 1);
    for (i = 0; i < 20; i++)  /* again */
        CHECK(ls_receive(&a, frames[i], len[i]).fresh == 0);
    CHECK(a.stats.duplicate == 20);
    CHECK(ls_missing(&a, 19) == 1);  /* our own slot */
    for (i = 0; i < 20; i++) {
        NetInput in = pad_of(0, (uint32_t)i), e = pad_of(1, (uint32_t)i);
        ls_submit_local(&a, (uint32_t)i, &in);
        CHECK(ls_take(&a, (uint32_t)i, out) == 0 && same(&out[1].in, &e));
    }
    CHECK(ls_take(&a, 5, out) == 0);  /* still held: read again */
    CHECK(ls_receive(&a, frames[3], len[3]).fresh == 0 && a.stats.old == 1);
    /* malformed */
    CHECK(ls_receive(&a, frames[0], len[0] - 1).type == -1);
    frames[0][1] = 7;
    CHECK(ls_receive(&a, frames[0], len[0]).type == -1);
    CHECK(ls_receive(&a, (const uint8_t *)"\x09\x00", 2).type == -1);
    CHECK(a.stats.bad == 3);
    /* too far ahead: beyond the window */
    {
        uint8_t far[LS_INPUT_HEADER + LS_INPUT_ENTRY] = {LS_MSG_INPUT, 1, 0, 0, 0, 0, 1};
        uint32_t f = a.next + LS_WINDOW;
        far[2] = (uint8_t)f, far[3] = (uint8_t)(f >> 8);
        CHECK(ls_receive(&a, far, sizeof far).fresh == 0 && a.stats.too_far == 1);
        f--, far[2] = (uint8_t)f, far[3] = (uint8_t)(f >> 8);
        CHECK(ls_receive(&a, far, sizeof far).fresh == 1);
    }
    /* our own echo is ignored */
    {
        uint8_t echo[64];
        size_t n = ls_encode_recent(&a, echo, sizeof echo);
        unsigned before = a.stats.ignored;
        CHECK(ls_receive(&a, echo, n).fresh == 0 && a.stats.ignored == before + 1);
    }
    /* a local sample too far ahead of the played fields */
    {
        NetInput in = pad_of(0, 0);
        CHECK(ls_submit_local(&a, a.next + LS_WINDOW, &in) == -1);
    }
}

/* The delay changes during the race: every field is scheduled exactly once, in order. */
static void test_delay_change(void)
{
    static Lockstep a;
    NetPad out[NET_MAX_SLOTS];
    uint32_t f;
    int filled = 0, dropped = 0;
    ls_init(&a, 1, 0, 2);
    ls_start(&a);
    for (f = 0; f < 200; f++) {
        NetInput in = pad_of(0, f);
        int n;
        if (f == 50)
            ls_set_delay(&a, 6);
        if (f == 100)
            ls_set_delay(&a, 1);
        if (f == 150)
            ls_set_delay(&a, 0);
        n = ls_submit_local(&a, f, &in);
        if (f == 50)
            { CHECK(n == 5); filled++; }
        if (f >= 100 && f < 105)
            { CHECK(n == 0); dropped++; }
        if (f == 105)
            CHECK(n == 1);
        if (f == 150)
            CHECK(n == 0);
        CHECK(a.local_last == (int64_t)f + a.delay || n == 0);
        CHECK(ls_take(&a, f, out) == 0);
        if (f < 50)
            CHECK(f < 2 || out[0].in.buttons == pad_of(0, f - 2).buttons);
        if (f >= 56 && f < 100)
            CHECK(out[0].in.buttons == pad_of(0, f - 6).buttons);
        if (f >= 106 && f < 150)
            CHECK(out[0].in.buttons == pad_of(0, f - 1).buttons);
        if (f >= 151)
            CHECK(out[0].in.buttons == pad_of(0, f).buttons);
    }
    CHECK(filled == 1 && dropped == 5);
    CHECK(ls_set_delay(&a, 999) == LS_MAX_DELAY && ls_set_delay(&a, -3) == 0);
}

/* A player who is late: the others wait at the first field, then catch up. */
static void test_late_player(void)
{
    static Lockstep a, late;
    uint8_t buf[2048];
    NetPad out[NET_MAX_SLOTS];
    uint32_t f;
    size_t n;
    ls_init(&a, 3, 0, 3), ls_init(&late, 3, 2, 3);
    ls_set_slot(&a, 1, NET_SLOT_AI), ls_set_slot(&late, 1, NET_SLOT_AI);  /* slot 1: the AI throughout */
    ls_start(&a);
    for (f = 0; f < 3; f++) {
        NetInput in = pad_of(0, f);
        ls_submit_local(&a, f, &in);
    }
    CHECK(ls_missing(&a, 2) == 0 && ls_missing(&a, 3) == 4);  /* slot 2 */
    CHECK(ls_take(&a, 3, out) == -1);
    ls_start(&late);  /* loads now */
    for (f = 0; f < 3; f++) {
        NetInput in = pad_of(2, f);
        ls_submit_local(&late, f, &in);
    }
    n = ls_encode_input(&late, 0, 6, buf, sizeof buf);
    CHECK(n == LS_INPUT_HEADER + 3 * LS_INPUT_ENTRY);  /* 0..2 are never sent */
    CHECK(buf[2] == 3 && buf[6] == 3);
    ls_receive(&a, buf, n);
    for (f = 0; f < 6; f++) {
        CHECK(ls_take(&a, f, out) == 0);
        CHECK(out[1].kind == NET_SLOT_AI && out[2].kind == NET_SLOT_HUMAN);
        if (f >= 3)
            CHECK(out[2].in.buttons == pad_of(2, f - 3).buttons);
    }
    CHECK(ls_missing(&a, 6) == 5);  /* both: a's own input reaches field 5 */
}

/* A player leaves: the server says the AI drives the slot from field 40. */
static void test_disconnect_to_ai(void)
{
    static Lockstep a, gone;
    uint8_t buf[64];
    NetPad out[NET_MAX_SLOTS];
    uint32_t f;
    ls_init(&a, 2, 0, 1), ls_init(&gone, 2, 1, 1);
    ls_start(&a), ls_start(&gone);
    for (f = 0; f < 39; f++) {
        NetInput in = pad_of(1, f);
        ls_submit_local(&gone, f, &in);
        if (f < 38)
            ls_receive(&a, buf, ls_encode_recent(&gone, buf, sizeof buf));
    }
    /* gone has fields 0..39 scheduled, but its last frame never arrived: a has 0..38 */
    for (f = 0; f < 39; f++) {
        NetInput in = pad_of(0, f);
        ls_submit_local(&a, f, &in);
        CHECK(ls_take(&a, f, out) == 0 && out[1].kind == NET_SLOT_HUMAN);
    }
    CHECK(ls_missing(&a, 39) == 2);
    CHECK(ls_after_last_received(&a, 1) == 39);  /* fields 36..38 were the last frame */
    CHECK(ls_after_last_received(&gone, 0) == 0);  /* nothing from slot 0 */
    ls_slot_ai_from(&a, 1, 40);
    CHECK(ls_missing(&a, 39) == 2);  /* field 39 still needs the player's input */
    ls_receive(&a, buf, ls_encode_input(&gone, 37, 3, buf, sizeof buf));
    CHECK(ls_take(&a, 39, out) == 0 && out[1].kind == NET_SLOT_HUMAN);
    for (f = 40; f < 100; f++) {
        NetInput in = pad_of(1, f);
        ls_submit_local(&gone, f, &in);  /* late frames from before the drop are ignored */
        ls_receive(&a, buf, ls_encode_recent(&gone, buf, sizeof buf));
        in = pad_of(0, f);
        ls_submit_local(&a, f, &in);
        CHECK(ls_take(&a, f, out) == 0);
        CHECK(out[1].kind == NET_SLOT_AI && out[1].in.buttons == 0 && out[1].in.lx == 0x80);
    }
    ls_slot_ai_from(&a, 1, 80);  /* a later announcement can't move it back */
    CHECK(a.slot[1].ai_from == 40);
}

static void test_hash_check(void)
{
    static Lockstep a;
    uint8_t buf[LS_HASH_SIZE];
    uint32_t field;
    ls_init(&a, 4, 1, 2);
    ls_local_hash(&a, 60, 0x1234567890ABCDEFull);
    ls_receive(&a, buf, ls_encode_hash(0, 60, 0x1234567890ABCDEFull, buf));
    ls_receive(&a, buf, ls_encode_hash(2, 120, 0x55, buf));  /* before ours */
    CHECK(!a.desynced);
    ls_local_hash(&a, 120, 0x55);
    ls_receive(&a, buf, ls_encode_hash(3, 120, 0x55, buf));
    CHECK(!a.desynced);
    ls_receive(&a, buf, ls_encode_hash(3, 180, 0x77, buf));
    ls_local_hash(&a, 180, 0x78);
    CHECK(a.desynced && a.desync_field == 180 && a.desync_slots == 8);
    ls_receive(&a, buf, ls_encode_hash(0, 180, 0x77, buf));
    CHECK(a.desync_slots == 9);
    ls_receive(&a, buf, ls_encode_hash(0, 240, 1, buf));
    ls_local_hash(&a, 240, 2);
    CHECK(a.desync_field == 180);  /* the first one stays */
    /* a wrapped-around entry: the 16 checkpoints after 60 reuse its slot */
    field = 60 + LS_HASHES * NET_HASH_PERIOD;
    ls_local_hash(&a, field, 9);
    ls_receive(&a, buf, ls_encode_hash(0, 60, 1, buf));  /* too old now: dropped */
    CHECK(a.hashes[1].field == field && a.hashes[1].have == 2);
}

static void test_auto_delay(void)
{
    float none[2] = {-1, -1}, lan[4] = {2, 3, 1, 2}, mix[4] = {30, 45, 120, 20}, one[1] = {40};
    float far[3] = {400, 380, -1};
    CHECK(ls_auto_delay(none, 2) == 1);
    CHECK(ls_auto_delay(lan, 4) == 2);   /* under a field each way, plus the slack */
    CHECK(ls_auto_delay(mix, 4) == 6);   /* (120 + 45) / 2 = 82.5 ms: 5 fields, +1 */
    CHECK(ls_auto_delay(one, 1) == 3);   /* 20 ms one way */
    CHECK(ls_auto_delay(far, 3) == 8);   /* the server's cap */
}

/* ---- a randomized network: loss, reordering, duplicates, NEED for what redundancy missed ---- */

typedef struct { int to; long when; size_t n; uint8_t data[1600]; } Packet;
static Packet *queue;
static int queued, queue_cap;
static unsigned rng = 12345;
static unsigned rnd(void) { rng = rng * 1103515245 + 12345; return rng >> 8; }

static void post(int to, long now, const uint8_t *data, size_t n, int loss, int jitter, int dup)
{
    int copies = (int)(rnd() % 100) < dup ? 2 : 1, i;
    if (!n)
        return;
    for (i = 0; i < copies; i++) {
        if ((int)(rnd() % 100) < loss)
            continue;
        if (queued == queue_cap)
            queue = realloc(queue, (queue_cap = queue_cap ? queue_cap * 2 : 256) * sizeof *queue);
        queue[queued].to = to;
        queue[queued].when = now + 2 + (long)(rnd() % (unsigned)(jitter + 1));
        queue[queued].n = n;
        memcpy(queue[queued].data, data, n);
        queued++;
    }
}

static void simulate(int peers, int delay, int loss, int jitter, int dup, uint32_t fields, const char *name)
{
    static Lockstep ls[NET_MAX_SLOTS];
    uint64_t digest[NET_MAX_SLOTS] = {0};
    uint32_t played[NET_MAX_SLOTS] = {0};
    long stuck[NET_MAX_SLOTS] = {0}, now;
    unsigned resends = 0;
    int p, q, i, done = 0;
    queued = 0;
    for (p = 0; p < peers; p++)
        ls_init(&ls[p], peers, p, delay), ls_start(&ls[p]);
    for (now = 0; now < 200000 && !done; now++) {
        /* deliver */
        for (i = 0; i < queued;) {
            if (queue[i].when <= now) {
                ls_receive(&ls[queue[i].to], queue[i].data, queue[i].n);
                queue[i] = queue[--queued];
            } else
                i++;
        }
        /* each peer plays at most one field per tick */
        done = 1;
        for (p = 0; p < peers; p++) {
            NetPad out[NET_MAX_SLOTS];
            uint8_t buf[1600];
            size_t n;
            if (played[p] >= fields)
                continue;
            done = 0;
            if (stuck[p] == 0) {  /* sample once per field */
                NetInput in = pad_of(p, played[p]);
                ls_submit_local(&ls[p], played[p], &in);
                n = ls_encode_recent(&ls[p], buf, sizeof buf);
                for (q = 0; q < peers; q++)
                    if (q != p)
                        post(q, now, buf, n, loss, jitter, dup);
            }
            if (ls_take(&ls[p], played[p], out) == 0) {
                for (q = 0; q < peers; q++)
                    digest[p] = net_hash64(&out[q], sizeof out[q], digest[p]);
                played[p]++;
                stuck[p] = 0;
            } else if (++stuck[p] % 8 == 0) {  /* waited a while: resend what others may lack */
                n = ls_encode_resend(&ls[p], buf, sizeof buf);
                CHECK(n <= LS_MAX_FRAME);
                for (q = 0; q < peers; q++)
                    if (q != p)
                        post(q, now, buf, n, loss, jitter, dup);
                resends++;
            }
        }
    }
    for (p = 0; p < peers; p++) {
        CHECK(played[p] == fields);
        CHECK(digest[p] == digest[0]);
    }
    printf("  %s: %u fields in %ld ticks, %u resends, peer 0 got %u frames (%u duplicate entries, %u old)\n",
           name, fields, now, resends, ls[0].stats.received, ls[0].stats.duplicate, ls[0].stats.old);
}

int main(void)
{
    test_hash();
    test_basic();
    test_receive_edges();
    test_delay_change();
    test_late_player();
    test_disconnect_to_ai();
    test_hash_check();
    test_auto_delay();
    simulate(4, 3, 0, 0, 0, 2000, "clean");
    simulate(4, 3, 20, 6, 10, 2000, "20% loss, jitter, duplicates");
    simulate(4, 1, 60, 20, 30, 1000, "60% loss, heavy reorder");
    simulate(2, 0, 10, 3, 0, 1000, "two players, no delay");
    free(queue);
    if (failures) {
        printf("test_lockstep: %d failures\n", failures);
        return 1;
    }
    printf("test_lockstep: ok\n");
    return 0;
}
