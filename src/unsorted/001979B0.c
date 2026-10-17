/* UNIT 0x001979B0 0x00197B68 */
typedef struct Body {
    int x0;
    char pad4[0xC];
    float pos[4];
} Body;

typedef struct Target {
    char pad0[0x10];
    float pos[4];
} Target;

typedef struct Entry {
    int x0;
    int x4;
    Body *x8;
    int xC;
    int x10;
} Entry;

typedef struct Table {
    char pad0[0x18];
    Entry entries[150];
    char padBDC[0xC00 - 0x18 - 150 * 0x14];
    int count;
} Table;

typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
} Obj;

typedef struct World {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Target *x792C;
} World;

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Camera {
    char pad0[0x80];
    Vec x80;
} Camera;

extern Table D_004348F0;
extern int D_00435A04[];
extern World D_002AF2C0;
extern Camera D_0037EA60;
extern char D_004B28FC[];
extern char D_003F53FD[];
extern int func_00196198(Table *);
extern void func_00231A08(void *, Vec *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void SetVec(Vec *v, Entry *e)
{
    v->x = e->x8->pos[0];
    v->y = e->x8->pos[1];
    v->z = e->x8->pos[2];
}

static inline Entry *GetEntry(Table *table, int index)
{
    int offset = index * sizeof(Entry);
    offset += (int)table;
    return (Entry *)(offset + 0x18);
}

void func_001979B0(Obj *obj)
{
    int count;
    int total;
    int i;
    int best;
    float bestDist;
    float distSq;
    Entry *e;
    Entry *entry;
    Vec pos;
    Vec d;
    Body *body;
    World *world;
    int selected;

    bestDist = 1.0e9f;
    best = -1;
    world = &D_002AF2C0;
    count = D_004348F0.count;
    total = func_00196198(&D_004348F0);
    if (count > 0 && total > 0) {
        i = 0;
        if (i < count) {
          e = &D_004348F0.entries[0];
          do {
            body = e->x8;
            if (body->x0 != 0) {
                pos.x = body->pos[0];
                pos.y = body->pos[1];
                pos.z = body->pos[2];
                __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(d) : "r"(world->x792C->pos), "r"(&pos) : "memory");
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(distSq) : "f"(d.x), "f"(d.y), "f"(d.z));
                if (distSq < bestDist) {
                    bestDist = distSq;
                    best = i;
                }
            }
            ++i;
            ++e;
          } while (i < count);
        }
    }
    if (best >= 0) {
        obj->x500 = best;
        entry = GetEntry(&D_004348F0, best);
        SetVec(&pos, entry);
        selected = GetEntry(&D_004348F0, best)->x4;
        obj->x504 = selected;
        D_00435A04[0] = selected;
        pos.z += 1.0f;
        func_00231A08(world->x40C0, &pos);
        COPY_VECTOR(D_0037EA60.x80, &pos);
        obj->x508 = 2;
        D_004B28FC[0] = 1;
        D_003F53FD[0] = 1;
    }
}
