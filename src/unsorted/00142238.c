/* UNIT 0x00142238 0x00142378 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Point { Vec pos; short x10; } Point;
typedef struct Body { char pad0[0x10]; Vec x10; } Body;
typedef struct Obj {
    char pad0[0x50];
    int x50, x54, x58, x5C, x60, x64;
    char pad68[0x20];
    Point *x88, *x8C, *x90;
    char pad94[0x0C];
    Vec xA0, xB0;
    char padC0[0x30];
    Body *xF0;
    char padF4[0x70];
    int x164;
} Obj;

extern char D_003C2230[];
extern void func_002182A8(Body *, int);
extern void func_001425E0(Obj *);
extern Point *func_00163DA0(void *, Point *, int);
extern void func_00141C00(Vec *, Vec *, Point *, Point *, float);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float Normalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

void func_00142238(Obj *obj, Point *point, Vec *pos)
{
    Vec delta;

    if (obj->xF0) {
        func_002182A8(obj->xF0, 0);
        COPY_VECTOR(obj->xF0->x10, pos);
        obj->x50 = 0;
        obj->x58 = 0;
        obj->x5C = 0;
        obj->x164 = 0;
        obj->x64 = 0;
        obj->x54 = 1;
        obj->x60 = 4;
        func_001425E0(obj);
        obj->x88 = point;
        obj->x8C = func_00163DA0(D_003C2230, point, 0);
        obj->x90 = func_00163DA0(D_003C2230, obj->x8C, 0);
        VecSub(&delta, obj->x8C, obj->x88);
        obj->xA0.w = Normalize(&obj->xA0, &delta);
        func_00141C00(&obj->xA0, &obj->xB0, obj->x8C, obj->x90, obj->x8C->x10);
    }
}
