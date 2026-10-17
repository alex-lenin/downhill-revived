/* UNIT 0x0015BC20 0x0015BCB8 */
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Quat;

struct Obj {
    char pad0[0x190];
    Quat x190;
    Quat x1A0;
};

extern void func_00222F60(Quat *, Quat *, Quat *);
extern void func_00223168(Quat *, Quat *);
extern void func_002235B0(Quat *, Quat *, Quat *);

void func_0015BC20(struct Obj *obj) {
    Quat q;
    Quat *dst = &obj->x190;
    Quat *src = &obj->x1A0;

    func_00222F60(&q, src, dst);
    /* 0.9999619f = cos(0.5 deg) */
    if (q.w < 0.9999619f) {
        q.w = 0.9999619f;
        func_00223168(&q, &q);
        func_002235B0(dst, &q, dst);
    } else {
        /* 16-byte vector copy (as sceVu0CopyVector) */
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0"
                          : "=m"(obj->x190) : "r"(src) : "$2");
    }
}
