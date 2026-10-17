/* UNIT 0x00165E68 0x001668C8 .sdata=0x29C7F0:0x29C7F4 */
#include "spatial_batch.h"
#include "impulse.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

/* x10.w = hit time */
typedef struct Item {
    char pad0[0x10];
    Vec x10;
    char pad20[0x10];
} Item;

typedef struct Group {
    Vec *x0;
    Vec *x4;
    int x8;
    float xC;
    float x10;
    int x14;
    int x18;
    int x1C;
    Item *x20;
    int x24;
} Group;

typedef struct Entry {
    char pad0[0x3C];
    int x3C;
    char pad40[0x20];
    Vec x60[6];
} Entry;

typedef struct Flags {
    unsigned long x0;
} Flags;

typedef struct Obj {
    char pad0[0xC0];
    Vec xC0;
    char padD0[0x100 - 0xD0];
    Flags *x100;
    char pad104[0x120 - 0x104];
    int x120;
    char pad124[0x140 - 0x124];
    char x140;
    char pad141[0x190 - 0x141];
    int x190;
    char pad194[0x1B4 - 0x194];
    int x1B4;
    char pad1B8[0x450 - 0x1B8];
    char x450[0x10];
} Obj;

#define BIT57 ((unsigned long)1 << 57)

int D_0029C7F0 = 0;

extern Entry D_003960A0[];
extern void func_0020B6D0(Vec *pos, int color, float radius);
extern int func_00238F30(void);
extern void func_00232B60(Obj *obj, Vec *state);
extern void func_00232BF8(Obj *obj, Vec *state);
extern void func_002327B0(Obj *obj, float dt);
extern void func_00231C10(Obj *obj);
extern Item *func_002116E8(Group *groups, int count, Item *after);
extern void func_001662B8(Obj *obj, Group *groups);
extern int func_00166298(Obj *obj, Item *hit);
extern void func_001CF858(void *a, Vec *pos, int type, float speed);
extern void func_00211280(Flags *flags);

static inline float Length(Vec *v)
{
    float len;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(len) : "f"(v->x), "f"(v->y), "f"(v->z));
    return len;
}

static inline float maxf(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline void Restore(Obj *obj, int saved)
{
    obj->x100->x0 = (obj->x100->x0 & ~BIT57) | ((unsigned long)saved << 57);
    func_00211280(obj->x100);
}

/* uses inline asm (EE FPU multiply-accumulate and sqrt) */
void func_00165E68(Obj *obj)
{
    Vec a;
    Vec b;
    Vec state[14];
    Group groups[8];
    Item items[8][8];
    Vec posA[8];
    Vec posB[8];
    Entry *entry;
    Item *hit;
    float inv;
    float remaining;
    float step;
    int count;
    int saved;
    int iter;
    int next;
    int i;
    int j;

    obj->x1B4++;
    entry = &D_003960A0[obj->x120];
    if (D_0029C7F0) {
        func_001668C8((ImpulseBody *)obj, (Vector4 *)&a, (const Vector4 *)&entry->x60[0]);
        func_001668C8((ImpulseBody *)obj, (Vector4 *)&b, (const Vector4 *)&entry->x60[1]);
        func_0020B6D0(&a, 0, entry->x60[0].w);
        func_0020B6D0(&b, 0, entry->x60[1].w);
    }
    obj->x140 = 0;
    inv = 1.0f / func_00238F30();
    count = entry->x3C;
    for (j = 0; j < count; j++) {
        groups[j].x8 = 0;
        groups[j].xC = 1.0f;
        groups[j].x0 = &posA[j];
        groups[j].x4 = &posB[j];
        groups[j].x20 = items[j];
        groups[j].x10 = entry->x60[j].w;
        groups[j].x1C = 8;
        groups[j].x14 = 0;
        groups[j].x18 = 0;
    }
    saved = (int)(obj->x100->x0 >> 57) & 1;
    obj->x100->x0 &= ~BIT57;
    remaining = 1.0f;
    iter = 0;
    do {
        func_00232B60(obj, state);
        step = inv * remaining;
        for (i = 0; i < count; i++)
            func_001668C8((ImpulseBody *)obj, (Vector4 *)&posA[i], (const Vector4 *)&entry->x60[i]);
        func_002327B0(obj, step);
        func_00231C10(obj);
        for (i = 0; i < count; i++)
            func_001668C8((ImpulseBody *)obj, (Vector4 *)&posB[i], (const Vector4 *)&entry->x60[i]);
        QuerySpatialBatchDiscard((SpatialBatchQuery *)(void *)(groups), count);
        hit = func_002116E8(groups, count, 0);
        if (hit == 0)
            break;
        next = iter + 1;
        if (next == 2)
            hit->x10.w = -1.0f;
        else
            func_00232BF8(obj, state);
        if (hit->x10.w < 0.0f) {
            func_001662B8(obj, groups);
        } else {
            for (;; hit = func_002116E8(groups, count, hit)) {
                if (hit != 0) {
                    func_002327B0(obj, step * hit->x10.w);
                    if (func_00166298(obj, hit)) {
                        remaining = maxf(0.0f, remaining - remaining * hit->x10.w);
                        break;
                    }
                    func_00232BF8(obj, state);
                } else {
                    func_002327B0(obj, step);
                    break;
                }
            }
        }
        func_001CF858(obj->x450, &hit->x10, obj->x120, Length(&obj->xC0));
        iter = next;
    } while (remaining > 0.0f && next < 2);
    if (obj->x120 == 0) {
        if (obj->x190 != 2)
            Restore(obj, saved);
    } else {
        Restore(obj, saved);
    }
}


extern int func_001663D8();

int func_00166298(Obj *obj, Item *hit)
{
    return func_001663D8(obj, hit);
}

typedef struct Particle_662B8 {
    char pad0[8];
    float x8;
    char padC[0x10];
    float x1C;
    Vec x20;
} Particle_662B8;

extern void func_00231A48(Obj *, Vec *);

static inline float Abs_662B8(float x)
{
    float r;
    __asm__ volatile("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

void func_001662B8(Obj *obj, Group *entries)
{
    Vec result, temp;
    Entry *world;
    int i, n;
    Particle_662B8 *particle;

    world = &D_003960A0[obj->x120];
    __asm__ volatile("sqc2 $vf0, %0" : "=m"(result));
    for (i = 0; i < world->x3C; i++) {
        n = entries[i].x24;
        particle = (Particle_662B8 *)entries[i].x20;
        while (n) {
            if (particle->x1C < 0.0f) {
                __asm__ volatile(
                    "lqc2 $vf11, 0x0(%1)\n\t"
                    "mfc1 $2,%2\n\t"
                    "qmtc2 $2,$vf12\n\t"
                    "vmulx.xyz $vf11,$vf11,$vf12x\n\t"
                    "sqc2 $vf11,%0"
                    : "=m"(temp)
                    : "r"(&particle->x20), "f"(particle->x8)
                    : "$2");
                {
                    float s, a, m, ma;
                    s = temp.x;
                    a = Abs_662B8(s);
                    m = result.x;
                    ma = Abs_662B8(m);
                    if (ma < a) result.x = s;
                }
                {
                    float s, a, m, ma;
                    s = temp.y;
                    a = Abs_662B8(s);
                    m = result.y;
                    ma = Abs_662B8(m);
                    if (ma < a) result.y = s;
                }
                {
                    float s, a, m, ma;
                    s = temp.z;
                    a = Abs_662B8(s);
                    m = result.z;
                    ma = Abs_662B8(m);
                    if (ma < a) result.z = s;
                }
            }
            n--;
            particle++;
        }
    }
    func_00231A48(obj, &result);
}

#define Vec Vec_663D8
#define Item Item_663D8
#define Frame Frame_663D8
#define Entry Entry_663D8
#define Obj Obj_663D8
#define Top4 Top4_663D8
#define D_003960A0 D_003960A0_663D8
#define func_0020B6D0 func_0020B6D0_663D8
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Item {
    char pad0[0xC];
    int xC;
    Vec x10;
    Vec x20;
} Item;

typedef struct Frame {
    char pad0[0x40];
    Vec x40;
    Vec x50;
} Frame;

typedef struct Entry {
    char pad0[0x4];
    float x4;
    float x8;
    float xC;
    char pad10[0x30];
    int x40;
    char pad44[0x3C];
    Vec x80[4];
} Entry;

typedef struct Obj {
    char pad0[0x38];
    const Matrix4 *x38;
    char pad3C[0xC4];
    Frame *x100;
    char pad104[0x1C];
    int x120;
    char pad124[0xC];
    Vec x130;
    char x140;
    char pad141[0xF];
    Vec x150[6];
    char pad1B0[0x4];
    int x1B4;
} Obj;

typedef struct Top4 {
    int count;
    short indices[4];
    float scores[4];
} Top4;

extern int D_0029C7F0;

extern Entry D_003960A0[] asm("D_003960A0");
extern void func_0020B6D0(Vec *pos, int color, float radius) asm("func_0020B6D0");
extern void func_00232A80(Obj *obj, Vec *out, Vec *pos);
extern void func_002320B0(Obj *obj, Vec *point, Vec *normal, float restitution, float lowFriction, float highFriction);

static inline Entry *GetEntry(Obj *obj)
{
    return &D_003960A0[obj->x120];
}

static inline void Query(Vec *out, Obj *obj, Vec *point)
{
    func_00232A80(obj, out, point);
}

static inline void Resolve(Obj *obj, Vec *point, Item *item, Entry *entry, float low, float high)
{
    func_002320B0(obj, point, &item->x20, entry->x4, low, high);
}

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float Dot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
                      : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline void Sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void Top4_Insert(Top4 *t, int index, float score)
{
    int j;
    int k;
    for (j = 0; j < t->count && !(score <= t->scores[j]); j++) {
    }
    if (j != 4) {
        if (t->count < 4) {
            t->count++;
        }
        for (k = t->count - 1; j < k; k--) {
            t->indices[k] = t->indices[k - 1];
            t->scores[k] = t->scores[k - 1];
        }
        t->scores[j] = score;
        t->indices[j] = index;
    }
}

int func_001663D8(Obj *obj, Item *__restrict__ item)
{
    struct {
        Vec out[4];
        Vec normalT;
        Top4 s;
    } ws;
    Vec before;
    Vec after;
    Entry *__restrict__ entry;
    Entry *__restrict__ entryLate;
    Frame *frame;
    Vec *o;
    Vec *mainDir;
    Vec *point;
    short *op;
    float *src;
    float *dst;
    float dot;
    float low;
    float high;
    float adjust;
    float magnitude0;
    float magnitude1;
    float magnitude2;
    float magnitude3;
    int count;
    int type;
    int i;
    int j;
    int k;
    int n;
    int i2;
    int i3;
    int filtered;

    entry = GetEntry(obj);
    if (item->xC == 0) {
        COPY_VECTOR(obj->x130, &item->x20);
        obj->x1B4 = 0;
        obj->x140 = 1;
    }
    count = entry->x40;
    entryLate = entry;
    if (count != 0) {
        mainDir = &item->x20;
        func_00221888(obj->x38, (const Vector4 *)mainDir, (Vector4 *)&ws.normalT);
        frame = obj->x100;
        dot = Dot(&frame->x50, mainDir);
        magnitude0 = Abs(dot);
        magnitude1 = Abs(dot);
        low = entryLate->x8 * magnitude0 * magnitude1;
        magnitude2 = Abs(dot);
        magnitude3 = Abs(dot);
        high = entryLate->xC * magnitude2 * magnitude3;
        if (obj->x120 == 0) {
            adjust = 1.0f;
            if (dot < 0.0f)
                adjust = -1.0f;
            adjust = (adjust + adjust) * (1.0f - dot);
            if (obj->x120 < count) {
                dst = &obj->x150[0].y;
                src = &entryLate->x80[0].y;
                n = count;
                do {
                    *dst = *src + adjust;
                    n--;
                    src += 4;
                    dst += 4;
                } while (n != 0);
            }
        } else if (obj->x120 == 1) {
            float dot2 = Dot(&frame->x40, &item->x20);
            adjust = 1.0f;
            if (dot2 < 0.0f)
                adjust = -1.0f;
            adjust = (adjust + adjust) * (1.0f - dot2);
            if (count > 0) {
                dst = &obj->x150[0].x;
                src = &entryLate->x80[0].x;
                n = count;
                do {
                    *dst = *src - adjust;
                    n--;
                    src += 4;
                    dst += 4;
                } while (n != 0);
            }
        }
        ws.s.count = 0;
        for (i = 0; i < count; i++) {
            float doti = Dot(&obj->x150[i], &ws.normalT);
            Top4_Insert(&ws.s, i, doti);
        }
        j = 0;
        i2 = 0;
        while (i2 < ws.s.count) {
                float contactMagnitude;
                Vec *accepted = &ws.out[j];
                func_001668C8((ImpulseBody *)obj, (Vector4 *)accepted, (const Vector4 *)&obj->x150[ws.s.indices[i2]]);
                Sub(&before, accepted, &ws.out[0]);
                contactMagnitude = Abs(Dot(&before, &item->x20));
                if (contactMagnitude < 0.05f) {
                    int accepted = j;
                    accepted++;
                    j = accepted;
                }
                i2++;
        }
        i3 = j - 1;
        if (i3 >= 0) {
            point = (Vec *)((unsigned int)(i3 << 4) + (unsigned int)ws.out);
            do {
                if (D_0029C7F0 != 0)
                    func_0020B6D0(point, 0, 0.25f);
                Query(&before, obj, point);
                Resolve(obj, point, item, entryLate, low, high);
                Query(&after, obj, point);
                if (before.x == after.x && before.y == after.y && before.z == after.z)
                    point--;
                else
                    point--;
                i3--;
            } while (i3 >= 0);
        }
    } else {
        point = &item->x10;
        Query(&ws.out[0], obj, point);
        func_002320B0(obj, point, point + 1, entryLate->x4, entryLate->x8, entryLate->xC);
        Query(&ws.out[1], obj, point);
    }
    return 1;
}
