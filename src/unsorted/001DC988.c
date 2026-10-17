/* UNIT 0x001DC988 0x001DCB48 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
#include "particle.h"
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Actor {
    long flags;
    char pad8[0x10 - 0x8];
    Vec pos;
} Actor;

typedef struct Source {
    unsigned int type : 6;
    unsigned int flags : 26;
    char pad4[0x10 - 0x4];
    char x10[0x1C - 0x10];
    int x1C;
} Source;

typedef struct Obj {
    char pad0[0x20];
    Vec pos;
    Actor *x30;
    Source *x34;
    char pad38[0x3C - 0x38];
    float x3C;
    float x40;
    char pad44[0x48 - 0x44];
    int x48;
    int x4C;
    int x50;
    int x54;
} Obj;

typedef struct Game {
    char pad0[0x240];
    char x240[0x5BF8 - 0x240];
    int *x5BF8;
} Game;

extern Game D_00398C40;
extern Actor *func_00208FB8(void);
extern void func_00218278(Actor *actor, int id);

extern void func_001DCCD8(Obj *obj);
#if defined(__mips__)
extern float func_00211980(Vec *a, Vec *b, int c, float limit);
#endif
extern float func_00222B68(float lo, float hi);
extern void func_001D46F0(void *a, Vec *b);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Notify(Game *g, Vec *p) { func_001D46F0(g->x240, p); }

static inline float minf(float a, float b) { float r; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

void func_001DC988(Obj *obj, int value)
{
    Game *game;
    Source *source;
    Actor *actor;
    Vec pos;
    float volume;
    char *sound;
    float height;

    source = obj->x34;
    if (source == 0) {
        return;
    }
    obj->x50 = value;
    source->type = 9;
    actor = func_00208FB8();
    obj->x30 = actor;
    if (actor != 0) {
        func_00218278(obj->x30, obj->x34->x1C);
        func_00221F20((char *)obj->x30 + 0x40);
        if (obj->x30 != 0) {
            *((char *)obj->x30 + 4) = 1;
            obj->x30->flags |= (long)0x8000 << 42;
        }
    }
    func_001DCCD8(obj);
    game = &D_00398C40;
    sound = obj->x34->x10;
    volume = 0.5f;
    if (*game->x5BF8 == 5) {
        volume = 1.0f;
    }
    func_0022CDC0((Vector4 *)(sound), 0x48, (Matrix4 *)(0), (int)(0), volume, (Vector4 *)(0));
    obj->x48 = 0;
    obj->x4C = 0;
    obj->x54 = 1;
    COPY_VECTOR(pos, &obj->x30->pos);
    pos.z -= obj->x3C;
    height = minf(10.0f, obj->x3C * 0.5f);
    obj->x40 = obj->x30->pos.z - CallGroundProbe(&pos, &obj->pos, 0, height);
    if (*game->x5BF8 == 5) {
        obj->x40 -= func_00222B68(-6.0f, 2.0f);
    } else {
        obj->x40 -= func_00222B68(0.0f, 6.0f);
    }
    Notify(&D_00398C40, &obj->x30->pos);
}
