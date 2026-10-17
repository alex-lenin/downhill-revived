/* UNIT 0x00180A10 0x00180E78 */
#include "rider_geometry.h"
#include "particle.h"
typedef struct Vec { float x, y, z, w; } __attribute__((aligned(16))) Vec;
typedef struct Matrix { Vec v[4]; } Matrix;
typedef struct Entry { int x0, x4, x8, xC; } Entry;
typedef struct Frame {
    long x0;
    char pad8[8];
    Vec pos;
    char pad20[0x20];
    Matrix matrix;
    Vec x80;
    Vec x90;
    char padA0[0x10];
    float xB0;
    char padB4[0xC];
    Matrix xC0;
} Frame;
typedef struct Effect { char pad0[0x2726]; short x2726; char pad2728[0x120]; void *x2848; } Effect;
typedef struct Obj {
    char pad0[8];
    Frame *x8;
    char padC[0xC4];
    Entry *xD0;
    char padD4[0x1C];
    char xF0[0x50];
    Vec x140;
    Vec x150;
    char pad160[0xB0];
    Effect *x210;
} Obj;
typedef struct Global { char pad[0x2A0]; int x2A0; } Global;
extern Global D_004B25A0;

extern float func_00238F40(void);
extern int func_00238F30(void);
extern void func_00236558(void *, void *, float);
extern void func_00180E78(void *, int);
extern int func_00211280(Frame *);
extern void func_00181390(void *);
extern void func_001805B0(void *);
extern Effect *func_0022DD88(int);
extern unsigned char func_00204230(Entry *);
extern void func_002038E0(Entry *, int);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst).v[0]), "=m"((dst).v[1]), "=m"((dst).v[2]), "=m"((dst).v[3]) \
                      : "r"(src) : "$8", "$9", "$10", "$11")

static __inline__ void sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a), "r"(b));
}

static __inline__ float dist2d(Vec *a, Vec *b)
{
    Vec d;
    float len;
    sub(&d, a, b);
    __asm__ volatile ("mula.s %1,%1\n\tmadd.s %0,%2,%2\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(d.x), "f"(d.y));
    return len;
}

void func_00180A10(Obj *obj)
{
    Vec pos;
    Matrix matrix;
    Vec tmp;
    int handle;
    Effect *effect;

    COPY_VECTOR(pos, &obj->x8->pos);
    COPY_MATRIX(matrix, &obj->x8->matrix);
    func_00260E08(obj->xF0, &obj->x140, &obj->x150, &tmp);
    func_00236558(&obj->x140, &tmp, func_00238F40());
    func_00180E78(obj, 0);
    func_00211280(obj->x8);
    func_00181390(obj);
    COPY_VECTOR(obj->x8->x90, &pos);
    COPY_MATRIX(obj->x8->xC0, &matrix);
    obj->x8->xB0 = 1.0f / (float)func_00238F30();
    obj->x8->x0 |= 0x0400000000000000L;
    func_001805B0(obj);
    if (D_004B25A0.x2A0 == 1) {
        if (obj->x210 == 0) {
            handle = func_0022CFB8((Vector4 *)(&obj->x8->pos), &obj->x8->matrix, 31, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
            if (handle != -1) {
                effect = func_0022DD88(handle);
                obj->x210 = effect;
                effect->x2848 = &obj->x210;
            }
        } else {
            obj->x210->x2726 = 60;
        }
    }
    if (D_004B25A0.x2A0 == 12) {
        Vec pt0 = { 3647.0f, -9053.0f, 6992.0f };
        Vec pt1 = { 9534.0f, -8080.0f, 2806.0f };
        Vec pt2 = { 10634.0f, -9135.0f, 1252.0f };
        if (dist2d(&obj->x8->pos, &pt0) < 25.0f) {
            Entry e = obj->xD0[0];
            if ((func_00204230(&e) ^ 1) & 0xFF) {
                Entry e2 = obj->xD0[0];
                func_002038E0(&e2, 1);
            }
        } else if (dist2d(&obj->x8->pos, &pt1) < 25.0f) {
            Entry e = obj->xD0[1];
            if ((func_00204230(&e) ^ 1) & 0xFF) {
                Entry e2 = obj->xD0[1];
                func_002038E0(&e2, 1);
            }
        } else if (dist2d(&obj->x8->pos, &pt2) < 25.0f) {
            Entry e = obj->xD0[2];
            if ((func_00204230(&e) ^ 1) & 0xFF) {
                Entry e2 = obj->xD0[2];
                func_002038E0(&e2, 1);
            }
        }
    }
}
