/* UNIT 0x002121E0 0x00212408 */
#include "spatial.h"
typedef SpatialEntry Entry;

typedef SpatialActor Actor;

typedef SpatialLink Node;

typedef SpatialCellIterator Iter;

typedef SpatialIterator Obj;

typedef Vector4 Vec;



extern void func_00212748(Actor *actor, Vec *lo, Vec *hi);
extern int func_00214550(void *a, void *b, void *c, void *d);
extern int func_00215140(Entry *entry, void *out, int flag);
extern void func_00215A18(Actor *actor, int flag);
extern int func_00215448(void *out, int flag);

int func_002121E0(Obj *p)
{
    Actor *actor;
    Iter *it;
    Entry *entry;
    struct { unsigned char v; } empty;
    struct { unsigned char v; } done;
    int v1;
    int res;
    int isDirect;

    {
        Iter *tmp;
        tmp = &p->cell;
        empty.v = 0;
        if (tmp->entry == 0) {
            empty.v = (tmp->node == 0);
        }
        if (empty.v & 0xFF) {
            func_00211DF0(tmp, p->x34, p->x30);
        } else {
            func_00211EC8(tmp);
        }
    }
    it = &p->cell;
    goto check;
    while (1) {
    check:
        done.v = 0;
        if (it->entry == 0) {
            done.v = (it->node == 0);
        }
        if (done.v & 0xFF) {
            break;
        }
        isDirect = (it->entry != 0);
        if (isDirect) {
            float *maxp;
            p->reference.actor = 0;
            p->reference.node = (SpatialMatrixNode *)&p->matrix;
            entry = it->entry;
            if (entry == 0) {
                entry = (Entry *)it->node->actor;
            }
            maxp = &entry->maxX;
            res = 0;
            if (p->min.x < entry->maxX && p->min.y < maxp[1] && maxp[2] > p->min.z &&
                entry->minX < p->max.x && entry->minY < p->max.y && entry->minZ < p->max.z) {
                res = 1;
            }
            v1 = ((int)p->cell.filter.x4.flags >> 17) & 1;
            if (res && func_00215140(entry, &p->collider, v1)) {
                return 1;
            }
        } else {
            Vec lo;
            Vec hi;
            actor = it->entry ? (Actor *)it->entry : it->node->actor;
            func_00212748(actor, &lo, &hi);
            if (func_00214550(p, &p->max.x, &lo, &hi)) {
                if (actor->x0 & 0x400000000000000UL) {
                    p->reference.actor = actor;
                    p->reference.node = 0;
                } else {
                    p->reference.actor = 0;
                    p->reference.node = (SpatialMatrixNode *)&p->matrix;
                }
                {
                    int f17 = p->cell.filter.x4.flags;
                    int b18 = (f17 >> 18) & 1;
                    int b17 = (f17 >> 17) & 1;
                    func_00215A18(actor, b18);
                    if (func_00215448(&p->collider, b17)) {
                    return 1;
                    }
                }
            }
        }
        func_00211EC8(it);
    }
    return 0;
}
