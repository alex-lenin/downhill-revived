/* UNIT 0x001DC548 0x001DC830 */
#include "include_asm.h"

#ifdef NONMATCHING
/* Non-matching: global-alloc swaps rider (s7 vs s3) with the upper-bound base, so the inner loop and the
   call-arg/spill sched differ; best diff 94/186 words */
typedef struct Score { char pad[0x18]; float x18; } Score;
typedef struct Entry {
    char pad0[0x30];
    Score *x30;
    Score *x34;
    char pad38[0x18];
    int x50; /* owning rider */
    int x54; /* entry state */
    char pad58[8];
} Entry;
typedef struct Rider { char pad0[0x792C]; Score *x792C; } Rider;
typedef struct State {
    Entry entries[100];
    Rider *x2580[4];
    int x2590;
    int x2594[4]; /* per-rider "done" flag */
    int x25A4[4]; /* per-rider upper entry index */
    int x25B4[4]; /* per-rider lower entry index */
    int x25C4;    /* entry count */
} State;
typedef struct Global { char pad[0x288]; int x288; } Global;
extern Global D_004B25A0;
extern int func_001DCB50(Entry *);
extern void func_001DC830(State *, int);
extern void func_001DC988(Entry *, int);
extern float func_001385A8(void *);

/* address of the int at byte offset `byte_offset` inside the State field at `field` */
static inline int *field_at(State *state, int field, int byte_offset)
{
    return (int *)((char *)state + field + byte_offset);
}

void func_001DC548(State *state)
{
    int *done;
    int rider, i, active;
    float previous, current, weight;
    Score *score;
    Entry *entry;
    int *upper, *lower, *start, *end;
    int next, offset, address, stride;

    rider = 0;
    if (D_004B25A0.x288 <= 0)
        return;
    done = state->x2594;
    do {
        next = rider + 1;
        offset = rider * 4;
        if (*(int *)((char *)done + offset) == 1)
            goto next_rider;
        upper = state->x25A4;
        lower = state->x25B4;
        active = 0;
        for (i = state->x25B4[rider]; i < state->x25A4[rider]; i++) {
            start = field_at(state, 0x25B4, offset);
            end = field_at(state, 0x25A4, offset);
            entry = &state->entries[i];
            if (entry->x50 == rider && (unsigned)(entry->x54 - 1) < 2) {
                active = 1;
                func_001DCB50(entry);
                /* entry left the 1..2 states: advance this rider's lower bound */
                if ((unsigned)(entry->x54 - 1) >= 2 && i == *start && i <= *end) {
                    do {
                        lower[rider]++;
                    } while (*(unsigned *)((char *)state + i * 0x60 + 0x50) != (unsigned)rider
                             && lower[rider] <= upper[rider]);
                }
            }
        }
        i = upper[rider];
        if (i < state->x25C4) {
            stride = 0x60;
            address = i * stride;
            address += (int)state;
            if (((Entry *)address)->x54 != 0) {
                func_001DC830(state, rider);
            } else {
                address = i * stride;
                address += (int)state;
                score = ((Entry *)address)->x30;
                if (score != 0 || (score = ((Entry *)address)->x34, previous = 0.0f, score != 0))
                    previous = score->x18;
                active = 1;
                current = state->x2580[rider]->x792C->x18;
                weight = func_001385A8((char *)state->x2580[rider] + 0x40C0);
                if (current - previous < weight * 200.0f + (1.0f - weight) * -25.0f) {
                    func_001DC988(&state->entries[i], rider);
                    func_001DC830(state, rider);
                }
            }
        }
        if (!active)
            *(int *)((char *)done + offset) = 1;
next_rider:
        rider = next;
    } while (rider < D_004B25A0.x288);
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_001DC548);
#endif
