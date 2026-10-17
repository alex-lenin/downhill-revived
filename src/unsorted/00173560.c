/* UNIT 0x00173560 0x00173688 */
struct Params {
    char pad0[0x870];
    float x870;
    float x874;
    float x878;
    float x87C;
    float x880;
};

struct Obj {
    int x0;
    int x4;
    int x8;
    struct Obj *xC;
    struct Params *x10;
    char pad14[0x3C];
    float x50[4];
    char pad60[0x20];
    float x80[4];
    char pad90[0x28];
    float xB8;
    float xBC;
};

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_00173560(struct Obj *obj) {
    float dot, target, angle, scale, t;
    float *a = obj->x80;
    float *b = obj->x50;

    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6"
                      : "=f"(dot)
                      : "f"(a[0]), "f"(b[0]), "f"(a[1]), "f"(b[1]), "f"(a[2]), "f"(b[2]));
    target = 0.0f;
    /* The do/while (0) blocks are needed: their loop notes act as scheduling
       barriers (abs.s before li.s, mul.s before li.s) and weight the refs of
       target/angle so target gets $f2. Reusing scale for the rate keeps the
       second fabsf from being CSE'd into the first. */
    do {
        if (dot < 0.0f) {
            if (obj->xB8 < 1e-10f) {
                COPY_VECTOR(obj->x50, obj->xC->x50);
            }
            goto done;
        }
        angle = obj->xBC;
    } while (0);
    scale = __builtin_fabsf(angle);
    if (!(scale > 1.5707964f)) {
        target = obj->x10->x870;
        if (obj->x4 == 2) {
            target = obj->x10->x874;
        }
        scale = target;
        t = __builtin_fabsf(angle);
        do {
            target = scale - scale * t * 0.63661975f; /* 2/pi */
        } while (0);
    }
done:
    if (obj->xB8 < target) {
        obj->xB8 += obj->x10->x87C;
        if (obj->xB8 > target) {
            obj->xB8 = target;
        }
    } else {
        obj->xB8 -= obj->x10->x880;
        if (obj->xB8 < target) {
            obj->xB8 = target;
        }
    }
}
