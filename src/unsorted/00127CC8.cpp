/* UNIT 0x00127CC8 0x00127F20 */
#include "animation.h"
struct Group { int x0; int count; char pad8[0x98]; unsigned short ids[40]; };
struct PoolEntry { char data[0x230]; };
struct Pool { struct PoolEntry entries[200]; unsigned char freeIndices[200]; unsigned char used; };
struct Node { char pad0[0x230]; struct PoolEntry *x230; };
struct Obj {
    char pad0[0x10];
    struct Group groups[24];
    char pad1690[0x5dd0 - 0x1690];
    struct Pool pool;
    char pad21419[0x21420 - 0x21419];
    int queued;
    char pad21424[0x21438 - 0x21424];
    int counts[24];
    char pad21498[0x215c8 - 0x21498];
    int slots[24][24];
    char pad21ec8[0x23b48 - 0x21ec8];
    int queue[24];
};
struct Member { AnimationController animation; char pad64[0x154]; };
extern char D_002839D8[];
struct Loader {
    char pad0[0x68];
    struct Member x68;
    void *x220;
    char pad224[0xC];
    Loader() { x220 = D_002839D8; func_001AC780(&x68.animation, 2, 1); }
    int func_001272F0(void *resource);
};

extern int D_004B2828[];
extern char D_00412510[], D_00288E48[];
extern int D_00768050[];

extern void *func_00163D40(void *, int);
extern struct Node *func_001930C8(int, void *);
extern int func_001931D0(int);
extern struct Node *func_00127F20(struct Obj *, int);
extern void func_00193208(int);
extern void func_001224F0(struct PoolEntry *, struct Node *, struct Group *, int, int);
extern int func_00193478(struct Node *);
extern char *func_00163D70(void *, void *);
extern void func_00254D08(char *, int, char *, int, int);

void func_00127CC8(struct Obj *obj, int index)
{
    int queued;
    int step;
    struct Group *group;
    int i;

    queued = obj->queued;
    if (queued < 24) {
        obj->queue[queued] = index;
        obj->queued = queued + 1;
        step = 1;
        group = &obj->groups[index];
        if (D_004B2828[0] >= 2) {
            step = 3;
        }
        for (i = 0; i < group->count; i += step) {
            Loader loader;
            void *resource;
            int handle;
            struct Node *node;
            struct PoolEntry *entry;

            resource = func_00163D40(D_00412510, group->ids[i]);
            if (resource == 0) {
                continue;
            }
            handle = loader.func_001272F0(resource);
            node = func_001930C8(handle, resource);
            if (node == 0) {
                node = func_00127F20(obj, func_001931D0(handle));
                func_00193208(handle);
            }
            if (node != 0) {
                struct Pool *pool = &obj->pool;
                if (pool->used < 200) {
                    entry = &pool->entries[pool->freeIndices[pool->used++]];
                } else {
                    entry = 0;
                }
                func_001224F0(entry, node, group, group->ids[i], handle);
                node->x230 = entry;
                obj->slots[index][obj->counts[index]++] = func_00193478(node);
            } else {
                func_00254D08(D_00288E48, index, func_00163D70(D_00412510, resource),
                              handle, D_00768050[handle]);
            }
        }
    }
}
