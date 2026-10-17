/* UNIT 0x00211EC8 0x00212048 */
#include "spatial.h"





static __inline__ int entry_mask(SpatialEntry *entry, unsigned short mask) { return (unsigned short)entry->flags & mask; }
static __inline__ int actor_mask(SpatialActor *actor, int mask) { return actor->xBC & mask; }

void func_00211EC8(SpatialCellIterator *iter) {
    int flags = iter->filter.x4.flags;
    int reverse = (flags >> 18) & 1;
    int skipA = (flags >> 16) & 1;
    int skipB = (flags >> 17) & 1;
    int step = reverse ? -1 : 1;
    SpatialActor *actor;

    while (iter->cur != iter->end) {
        if ((*iter->cur & 1) != reverse) {
            iter->cur = iter->end;
            break;
        }
        {
            SpatialEntry *entry = (SpatialEntry *)(iter->base + ((*iter->cur >> 7) * 4));
            int id = iter->id;
            iter->entry = entry;
            if (entry->x18 != id && !(skipA && (entry->flags & 0x1000000))
                && !(skipB && (entry->flags & 0x2000000))
                && entry_mask(entry, (unsigned short)iter->filter.x4.flags)) {
                entry->x18 = id;
                return;
            }
        }
        iter->cur += step;
    }
    iter->entry = 0;
    for (; iter->node; iter->node = iter->node->next) {
        actor = iter->node->actor;
        if (actor && *((unsigned char *)actor + 4) && (actor->x0 & (0x8000L << 42)) && *(int *)actor
            && actor->x80 != iter->id && actor_mask(actor, iter->filter.x0)) {
            actor->x80 = iter->id;
            return;
        }
    }
}
