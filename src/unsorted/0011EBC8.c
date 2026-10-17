/* UNIT 0x0011EBC8 0x0011EDA8 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct Model {
    char pad0[0x10];
    Vec4 x10;
} Model;

typedef struct Link {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x20];
    void *x40;
} Link;

typedef struct PathRef {
    char *path;
    Vec4 *node;
    short x8;
    short xA;
} PathRef;

typedef struct Obj {
    char pad0[0x10];
    Model *x10;
    Model *x14;
    char pad18[0xE4];
    PathRef xFC;
    char pad108[0x8];
    Link x110;
    float x160;
    float x164;
    char pad168[0x8];
    Vec4 x170;
    Vec4 x180;
    char pad190[0x30];
    Vec4 x1C0;
    Vec4 x1D0;
    Vec4 x1E0;
    Vec4 x1F0;
    char pad200[0x4C];
    int x24C;
    char pad250[0x18];
    union {
        unsigned long flags;
        struct {
            int pad;
            short x26C;
        } h;
    } x268;
} Obj;

extern char D_003041B0[];

extern Vec4 *func_00163D40(void *, int);
extern void func_002609B0(Link *);
#if defined(__mips__)
extern float func_00211980(Vec4 *, Vec4 *, int, float);
#endif
extern void func_00121200(Obj *, Vec4 *, Vec4 *);
extern void func_00120AA8(Obj *, Vec4 *, Vec4 *, int);
extern void func_002182A8(Model *, int);
extern void func_00211280(Model *);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* copies xyz, keeps the destination's w */
static inline void CopyXYZ(Vec4 *dst, Vec4 *src)
{
    float w = dst->w;

    COPY_VECTOR(*dst, src);
    dst->w = w;
}

static inline float Dot(Vec4 *a, Vec4 *b)
{
    float result;

    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6"
        : "=f" (result)
        : "f" (a->x), "f" (b->x), "f" (a->y), "f" (b->y), "f" (a->z), "f" (b->z));
    return result;
}

static inline void Link_Attach(Link *link, PathRef *ref)
{
    link->x40 = ref;
    func_002609B0(link);
}

static inline void PathRef_Init(PathRef *ref, char *path, Vec4 *node)
{
    ref->path = path;
    if (node == 0) {
        node = func_00163D40(path, 0);
    }
    ref->node = node;
    ref->xA = -1;
    ref->x8 = -1;
}

void func_0011EBC8(Obj *obj, Vec4 *start)
{
    Vec4 normal;
    Vec4 dir;
    Vec4 pos;
    Vec4 unit;
    Vec4 *plane;

    if (obj->x10 == 0) {
        return;
    }
    PathRef_Init(&obj->xFC, D_003041B0, start);
    Link_Attach(&obj->x110, &obj->xFC);
    CopyXYZ(&dir, &obj->x110.x10);
    COPY_VECTOR(pos, start);
    pos.z -= CallGroundProbe(&pos, &normal, 0, 15.0f);
    plane = &obj->x1D0;
    COPY_VECTOR(obj->x1D0, &normal);
    obj->x1D0.w = Dot(&normal, &pos);
    COPY_VECTOR(obj->x1C0, plane);
    obj->x268.h.x26C = 0;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m" (unit));
    if (obj->x14 != 0) {
        COPY_VECTOR(obj->x14->x10, &pos);
        func_00121200(obj, &dir, &normal);
    }
    COPY_VECTOR(obj->x10->x10, &pos);
    func_00120AA8(obj, &dir, &normal, 1);
    obj->x160 = pos.x;
    obj->x164 = pos.y;
    CopyXYZ(&obj->x170, &unit);
    CopyXYZ(&obj->x180, &dir);
    obj->x24C = 0;
    __asm__ volatile ("sqc2 $vf0, %0" : "=m" (obj->x1E0));
    COPY_VECTOR(obj->x1F0, &pos);
    obj->x268.flags &= ~((unsigned long)1 << 50);
    func_002182A8(obj->x10, 1);
    if (obj->x14 != 0) {
        func_002182A8(obj->x14, 1);
    }
    func_00211280(obj->x10);
}
