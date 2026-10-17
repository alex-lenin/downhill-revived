/* UNIT 0x001812F8 0x00181390 */
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Quat;
struct Obj { char pad0[0x180]; Quat x180; Quat x190; };
extern void func_00222F60(Quat *, Quat *, Quat *);
extern void func_00223168(Quat *, Quat *);
extern void func_002235B0(Quat *, Quat *, Quat *);
void func_001812F8(struct Obj *obj) {
    Quat q;
    Quat *target = &obj->x190;
    Quat *cur = &obj->x180;
    func_00222F60(&q, target, cur);
    if (q.w < 0.99999f) {
        q.w = 0.99999f;
        func_00223168(&q, &q);
        func_002235B0(cur, &q, cur);
    } else {
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0"
                          : "=m"(obj->x180) : "r"(target) : "$2");
    }
}
