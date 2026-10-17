/* UNIT 0x00127458 0x001274F0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Quat;

struct Obj {
    char pad0[0x100];
    Quat x100;
    Quat x110;
};

extern void func_00222F60(Quat *, Quat *, Quat *);
extern void func_00223168(Quat *, Quat *);
extern void func_002235B0(Quat *, Quat *, Quat *);

void func_00127458(struct Obj *obj) {
    Quat q;
    Quat *target = &obj->x110;
    Quat *cur = &obj->x100;

    func_00222F60(&q, target, cur);
    if (q.w < 0.9999619f) {
        q.w = 0.9999619f;
        func_00223168(&q, &q);
        func_002235B0(cur, &q, cur);
    } else {
        /* 16-byte vector copy (as sceVu0CopyVector) */
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0"
                          : "=m"(obj->x100) : "r"(target) : "$2");
    }
}
