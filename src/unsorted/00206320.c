/* UNIT 0x00206320 0x002063F0 */
#include "animation.h"
struct Info {
    char pad0[0x10];
    short index;
};

struct Entry {
    unsigned int flags;
    struct Info *info;
};

struct List {
    char pad0[0x8];
    void *current;
};

struct Owner {
    char pad0[0x24];
    struct List *list;
};

extern struct Entry *func_00206DD0(struct List *);
extern void func_00206258(struct Entry *, struct Owner *);

void func_00206320(const AnimationHandle *input)
{
    AnimationHandle h = *input;
    AnimationHandle *p = &h;
    struct Owner *owner;
    struct List *list;
    struct Entry *entry;
    AnimationHandle sub;
    unsigned int type;

    owner = (struct Owner *)p->x0;
    if (owner != 0 && p->x4 != 0) {
        list = owner->list;
        entry = func_00206DD0(list);
        if (entry != 0) {
            type = entry->flags & 0x3F;
            if (type == 0x21) {
                sub.x0 = (AnimationOwner *)owner;
                sub.x8 = 0;
                sub.x4 = (AnimationTrack *)entry;
                sub.xC = entry->info->index;
                func_00203CB8(&sub);
            } else if (type == 0x22) {
                func_00206258(entry, owner);
            }
        }
        list->current = p->x4;
    }
}
