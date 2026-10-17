/* UNIT 0x001B0F80 0x001B1068 */
#include "rider.h"
typedef RiderComponent Level;
typedef struct Slot {
    char pad0[0x10C];
    Level *level;
} Slot;

typedef struct Slots {
    Slot slot[4];
    Level *level;
} Slots;

extern void func_001B11C8(Slot *slot, unsigned int id, int a, int b, int c);

void func_001B0F80(Slots *s, Level *level)
{
    unsigned int id;
    int x;
    Slot *p;
    Slot *end;
    Slot *last;

    id = *(unsigned int *)level->node->x0 >> 18;
    x = level->rider->riderIndex;
    s->level = level;
    p = s->slot;
    end = p + 4;
    do {
        p->level = level;
        p++;
    } while ((int)p < (int)end);
    last = &s->slot[3];
    func_001B11C8(&s->slot[0], id, 1, 0, x);
    func_001B11C8(&s->slot[1], id, 1, 1, x);
    func_001B11C8(&s->slot[2], id, 0, 0, x);
    func_001B11C8(last, id, 0, 1, x);
}
