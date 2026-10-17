/* UNIT 0x0010B998 0x0010BD10 .rodata=0x2888C0:0x2888F0 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct World { char pad0[0x792c]; char *x792c; char pad7930[0x795c - 0x7930]; float x795c; char pad7960[0x798c - 0x7960]; int x798c; } World;
typedef struct Item { char pad0[0x10]; unsigned char x10; char pad11[3]; unsigned short x14; } Item;
typedef struct Obj {
    World *x0;
    char pad4[4];
    World *x8;
    char padc[0x220 - 0xc];
    Vec x220;
    Vec x230;
    char pad240[0x3a0 - 0x240];
    int x3a0;
    char pad3a4[0x408 - 0x3a4];
    int x408;
    char pad40c[0x414 - 0x40c];
    int x414;
    char pad418[0x440 - 0x418];
    long x440;
} Obj;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float maxf(float a, float b) { float r; __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }

static inline void setVec(Vec *v, float x, float y, float z) { v->x = x; v->y = y; v->z = z; }

extern float func_00222B68(float, float);
extern void func_0010BD10(Obj *);

void func_0010B998(Obj *obj, Item *item, Vec *out)
{
    float scale;
    float r;
    Vec *target;
    char *matrix;

    if (item == 0)
        return;

    if ((item->x14 >> 5) & 1) {
        switch (obj->x0->x798c) {
        case 0:
        case 1:
            setVec(&obj->x230, 0.0f, 10.0f, 0.0f); break;
        case 2:
            setVec(&obj->x230, -5.0f, -5.0f, 0.0f); break;
        case 3:
            setVec(&obj->x230, 5.0f, -5.0f, 0.0f); break;
        case 4:
            setVec(&obj->x230, 0.0f, -10.0f, 0.0f); break;
        case 5:
            setVec(&obj->x230, -8.0f, -10.0f, 0.0f); break;
        case 6:
            setVec(&obj->x230, 8.0f, -10.0f, 0.0f); break;
        case 7:
            setVec(&obj->x230, -3.0f, -15.0f, 0.0f); break;
        case 8:
            setVec(&obj->x230, 3.0f, -15.0f, 0.0f); break;
        case 9:
            setVec(&obj->x230, -8.0f, -17.0f, 0.0f); break;
        case 10:
            setVec(&obj->x230, 8.0f, -17.0f, 0.0f); break;
        default:
            setVec(&obj->x230, 0.0f, -20.0f, 0.0f); break;
        }
        COPY_VECTOR(obj->x220, &obj->x230);
    } else if (obj->x440 & ((long)0x8000 << 38)) {
        setVec(&obj->x230, obj->x8->x795c, 0.0f, 0.0f);
    } else {
        scale = item->x10;
        if ((obj->x3a0 >> 11) & 1) {
            scale = 0.5f;
        } else if (obj->x0->x798c == 1) {
            scale = 1.0f;
        } else if (obj->x0->x798c < 4) {
            scale = maxf(2.0f, scale);
        } else if (obj->x0->x798c < 6) {
            scale = maxf(4.0f, scale);
        } else {
            scale = maxf(7.0f, scale + scale);
        }
        r = func_00222B68(scale * 0.5f, scale);
        if (obj->x0->x798c & 1)
            setVec(&obj->x230, r, 0.0f, 0.0f);
        else
            setVec(&obj->x230, -r, 0.0f, 0.0f);
        if (obj->x408 > 0xb4 && obj->x414 > 0x12c) {
            if (obj->x230.x > 0.0f)
                obj->x230.x = scale * 1.5f;
            else
                obj->x230.x = -scale * 1.5f;
        }
    }
    func_0010BD10(obj);
    target = &obj->x220;
    matrix = obj->x0->x792c + 0x40;
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(target), "r"(matrix));
}
