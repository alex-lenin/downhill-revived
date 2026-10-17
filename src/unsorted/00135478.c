/* UNIT 0x00135478 0x00135690 */
typedef struct Obj {
    char pad0[0x158];
    char *x158;
    char pad15C[0xD4];
    float dir[4]; /* 0x230 */
} Obj;

typedef struct { float x, y, z, w; } Vec4;

extern float fabsf(float);
extern void *func_00138558(void *);
extern int func_00138560(Obj *);
extern int func_00138570(Obj *);
extern void func_001EABE0(void *);
extern void func_00221888(void *mtx, void *in, void *out);
extern float func_00235EA8(float current, float target, float step, float minStep);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_00135478(Obj *o)
{
    char *p = o->x158 + 0x40C0;
    char *r = func_00138558(p);
    float *target = (float *)(r + 0x20);
    float d, step, side, absSide;
    float *a;
    float x, y, z, len, inv;

    if (*(short *)(p + 0x4A4) == 0) {
        char *q = *(char **)(o->x158 + 0x792C);
        float *n = (float *)(q + 0x60);
        Vec4 local;

        __asm__ volatile ("mula.s %1,%3\n\tmadda.s %2,%4\n\tmadd.s %0,%5,%6"
                          : "=f"(d)
                          : "f"(target[0]), "f"(target[1]), "f"(n[0]), "f"(n[1]),
                            "f"(target[2]), "f"(n[2]));
        if (d > 0.7f)
            goto snap;
        func_00221888(q + 0x40, target, &local);
        /* Zero-byte label: its output gives absSide its own register instead of
           tying abs.s to side (retail: abs.s $f1,$f0, local.y in $f2). */
        if (local.y > 0.0f
            && (side = local.x,
                ({ __asm__ volatile ("L35478_a0:" : "=f"(absSide)); }),
                absSide = fabsf(side),
                absSide < local.y)) {
        snap:
            COPY_VECTOR(o->dir, target);
        } else {
            if (d > 0.0f) {
                step = d * 0.001f;
            } else {
                step = 0.00001f;
                if (d > -0.5f)
                    step = 0.0001f;
            }
            o->dir[0] = func_00235EA8(o->dir[0], target[0], step, 0.00001f);
            o->dir[1] = func_00235EA8(o->dir[1], target[1], step, 0.00001f);
            o->dir[2] = func_00235EA8(o->dir[2], target[2], step, 0.00001f);
            a = o->dir;
            x = a[0];
            y = a[1];
            z = a[2];
            __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\t"
                              "lui $3,0x3f80\n\tmtc1 $3,%0\n\tnop\n\tdiv.s %0,%0,%1"
                              : "=&f"(inv), "=&f"(len)
                              : "f"(x), "f"(y), "f"(z)
                              : "$3");
            a[0] = x * inv;
            a[1] = y * inv;
            a[2] = z * inv;
        }
    }
    if (!func_00138560(o) && func_00138570(o))
        func_001EABE0(o->x158 + 0x5FF0);
}
