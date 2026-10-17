/* UNIT 0x00127F20 0x001284F0 .rodata=0x288E90:0x288F70 */
#include "soft_double.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));
typedef struct Transform { char pad0[0x10]; Vec4 x10; } Transform;
typedef struct Kind { unsigned int flags; } Kind;
typedef struct Shape { Kind *kind; } Shape;
typedef struct VTable { char pad0[0x20]; short delta; short pad22; Transform *(*get)(void *); } VTable;
typedef struct PoolObj { char pad0[0x230]; } PoolObj;
typedef struct Handle { VTable *vt; Shape *x4; char pad8[0x230 - 0x8]; PoolObj *x230; char pad234[0x24C - 0x234]; unsigned char x24C; } Handle;
typedef struct Pool { PoolObj entries[200]; unsigned char freeIndices[200]; unsigned char freeCount; char pad1B649[7]; } Pool;
typedef struct World { char pad0[0x5dd0]; Pool pool; int count; char pad21424[0x14]; int counts[100]; int handles[100][24]; int active[100]; } World;

extern Handle *func_00193498(int index);
extern void func_00122CF0(PoolObj *obj);

extern void func_00254D08(const char *fmt, ...);
extern int D_0029E4A0;
extern float D_0029C228[];
extern struct { char pad0[0x5019C]; float x5019C; } D_002AF2C0;
extern struct { unsigned char pad0[0x1008D]; unsigned char x1008D; } D_00412510;
typedef struct Debug { unsigned char pad0[0x8D]; unsigned char x8D; } Debug;
extern Debug D_00422510;
static inline int DebugEnabled(Debug *d) { return d->x8D; }

static inline Transform *GetTransform(Handle *h) {
    return h->vt->get((Handle *)((char *)h + h->vt->delta));
}

static inline void Release(Pool *pool, PoolObj *obj) {
    int index;
    func_00122CF0(obj);
    index = obj - pool->entries;
    pool->freeIndices[--pool->freeCount] = index;
}

static inline void RemoveFromGroup(World *world, int id, int index) {
    world->counts[id]--;
    if (index < world->counts[id]) {
        for (; index < world->counts[id]; index++)
            world->handles[id][index] = world->handles[id][index + 1];
    }
}

Handle *func_00127F20(World *world, int id1)
{
    int i, j, id2, *p;
    Handle *handle;
    Handle *best;
    float bestDist;

    {
    int i, j, id2, *p;
    Handle *handle;
    for (i = 0; i < world->count - 1; i++) {
        id2 = world->active[i];
        p = (int *)((char *)world + (id2 * 0x60 + 0x215C8));
        for (j = 0; j < world->counts[id2]; j++, p++) {
            handle = func_00193498(*p);
            if ((handle->x4->kind->flags >> 18) == id1 && handle->x24C != 0) {
                Release(&world->pool, handle->x230);
                world->counts[id2]--;
                if (j < world->counts[id2]) {
                    for (; j < world->counts[id2]; j++, p++)
                        *p = p[1];
                }
                if (DebugEnabled(&D_00422510))
                    func_00254D08("  Stealing ped with Id1 %d\n", id1);
                return handle;
            }
        }
    }
    }
    bestDist = D_0029C228[0];
    best = 0;
    for (i = 0; i < D_0029E4A0; i++) {
        handle = func_00193498(i);
        if ((handle->x4->kind->flags >> 18) == id1) {
            if (bestDist < GetTransform(handle)->x10.z) {
                best = handle;
                bestDist = GetTransform(best)->x10.z;
            }
        }
    }
    if (best != 0) {
        if (bestDist < D_002AF2C0.x5019C + 50.0f)
            best = 0;
    }
    if (best != 0) {
    int found;
    int i, j, id2, *p;
    PoolObj *obj = best->x230;
    Pool *pool;
    int index;
    found = 0;
    func_00122CF0(obj);
    pool = &world->pool;
    index = obj - pool->entries;
    pool->freeIndices[--pool->freeCount] = index;
    for (i = 0; i < world->count - 1 && !found; i++) {
        id2 = world->active[i];
        p = (int *)((char *)world + (id2 * 0x60 + 0x215C8));
        for (j = 0; j < world->counts[id2]; j++, p++) {
            if (func_00193498(*p) == best) {
                found = 1;
                world->counts[id2]--;
                if (j < world->counts[id2]) {
                    for (; j < world->counts[id2]; j++, p++)
                        *p = p[1];
                }
                break;
            }
        }
    }
    if (!found) {
        best = 0;
        if (D_00412510.x1008D)
            func_00254D08("  Stealing highestPed %d at (%f %f %f), but couldn't find active group\n", id1, func_001FD300(GetTransform(best)->x10.x),
                func_001FD300(GetTransform(best)->x10.y), func_001FD300(GetTransform(best)->x10.z));
    } else {
        if (D_00412510.x1008D)
            func_00254D08("  Stealing highestPed with Id1 %d at (%f %f %f)\n", id1, func_001FD300(GetTransform(best)->x10.x),
                func_001FD300(GetTransform(best)->x10.y), func_001FD300(GetTransform(best)->x10.z));
    }
    } else {
        if (D_00412510.x1008D)
            func_00254D08("  Tried to steal ped with id1 %d, but couldn't find one\n", id1);
    }
    return best;
}

