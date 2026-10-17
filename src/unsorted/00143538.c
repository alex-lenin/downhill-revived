/* UNIT 0x00143538 0x00143938 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Item {
    char pad0[0x50];
    int active;
    char pad54[0x9C];
    void *xF0;
    char padF4[0x78];
    int x16C;
} Item;

typedef struct Rider {
    char pad0[0x40C0];
    char x40C0[0xC0];
    Vec x4180;
    char pad4190[0x379C];
    char *x792C;
} Rider;

typedef struct World {
    char pad0[0x50130];
    Rider *x50130[1];
} World;

typedef struct Obj {
    int state;
    int count;
    int x8;
    char padC[4];
    char *x10;
    char pad14[0xC];
    Vec x20;
    Vec x30;
    int x40;
    int x44;
    int x48;
    char pad4C[4];
    Vec x50;
    float x60;
    int x64;
    float x68;
    float x6C;
    Item items[10];
    char padED0[0xA0];
    unsigned int xF70;
} Obj;

typedef struct PathTable {
    char _dummy;
} PathTable;
extern PathTable D_003C2230[];
extern World D_002AF2C0;
extern int D_0029DBAC;

extern int func_00163B70(void *);
extern Vec *func_00163D40(void *, int);
extern float func_001385A8(void *);
extern void func_00143390(Obj *, void *);
extern void func_00142378(Item *, Vec *);
extern void func_00143938(Obj *);
extern void func_00143A80(Obj *);
extern void func_00141F58(Item *);
extern void func_00141E20(Item *, Vec *);
extern void func_00142508(Item *);
extern void func_00143250(Obj *);
extern void func_001D2F38(unsigned int *, void *);
extern void func_001D2F10(void *);

static inline void copy_vector(Vec *dst, Vec *src)
{
    typedef unsigned int U128 __attribute__((mode(TI)));
    *(U128 *)dst = *(U128 *)src;
}

static inline void sub_vec(Vec *out, void *a, void *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)"
                      : : "r"(out), "r"(a), "r"(b) : "memory");
}

static inline float vec_length(Vec *v)
{
    float length;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(length) : "f"(v->x), "f"(v->y), "f"(v->z));
    return length;
}

void func_00143538(Obj *obj, int update)
{
    Vec diff;
    Rider *rider;
    Rider *bestRider;
    char *riderPos;
    Item *item;
    Item *firstItem;
    Vec *node;
    float distance;
    float best;
    float t;
    int nodeCount;
    int i;
    int riderIndex;
    int activeCount;

    switch (obj->state) {
    case 1:
        obj->x48 = -1;
        obj->x60 = 1000.0f;
        obj->state = 2;
        obj->x40 = 0;
        obj->x44 = 0;
        break;

    case 2:
        if (!update) {
            break;
        }
        bestRider = 0;
        riderIndex = 0;
        nodeCount = func_00163B70(D_003C2230);
        obj->x64 = -1;
        rider = D_002AF2C0.x50130[riderIndex];
        best = obj->x6C + 1.0f;
        if (rider != 0) {
            do {
                for (i = 0; i < nodeCount; i++) {
                    riderPos = rider->x792C + 0x10;
                    node = func_00163D40(D_003C2230, i);
                    sub_vec(&diff, riderPos, node);
                    diff.w = vec_length(&diff);
                    t = 1.0f - func_001385A8((char *)rider + 0x40C0);
                    diff.w += (obj->x6C - obj->x68) * t;
                    if (diff.w < best) {
                        obj->x64 = i;
                        best = diff.w;
                        bestRider = rider;
                    }
                }
                riderIndex++;
                rider = D_002AF2C0.x50130[riderIndex];
            } while (rider != 0);
        }
        if (obj->x64 >= 0) {
            obj->state = 3;
            if (bestRider != 0) {
                obj->x10 = bestRider->x792C;
                copy_vector(&obj->x50, &bestRider->x4180);
            }
        }
        break;

    case 3:
        node = func_00163D40(D_003C2230, obj->x64);
        func_00143390(obj, node);
        obj->state = 4;
        obj->x8 = 0x708;
        for (i = 0; i < obj->count; i++) {
            func_00142378(&obj->items[i], &obj->x50);
        }
        break;

    case 4:
        i = 0;
        func_00143938(obj);
        activeCount = 0;
        func_00143A80(obj);
        firstItem = obj->items;
        if (obj->count > 0) {
            item = firstItem;
            do {
                if (item->active) {
                    if ((D_0029DBAC + i) & 1) {
                        func_00141F58(item);
                    } else {
                        func_00141E20(item, &obj->x20);
                    }
                    func_00142508(item);
                    activeCount++;
                }
                i++;
                item++;
            } while (i < obj->count);
        }
        if (activeCount == 0) {
            obj->state = 1;
        }
        if (firstItem->x16C == 2) {
            sub_vec(&diff, &obj->x30, obj->x10 + 0x10);
            distance = vec_length(&diff);
            if (obj->x6C < distance) {
                func_00143250(obj);
            }
        }
        break;
    }

    {
        int scanIndex = 0;
        void *sound = 0;
        char *scanBase;

        for (; scanIndex < obj->count; scanIndex++) {
            scanBase = (char *)obj + scanIndex * 0x170;
            if (*(int *)(scanBase + 0xC0)) {
                sound = *(void **)(scanBase + 0x160);
                break;
            }
        }
        if (obj->state == 4 && sound != 0) {
            func_001D2F38(&obj->xF70, (char *)sound + 0x10);
        } else {
            func_001D2F10(&obj->xF70);
        }
    }
}
