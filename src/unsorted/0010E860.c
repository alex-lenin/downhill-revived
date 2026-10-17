/* UNIT 0x0010E860 0x0010E918 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Pair {
    u128 a;
    u128 b;
} Pair;

typedef struct Owner {
    char pad0[0x792C];
    char *x792C;
} Owner;

typedef struct Obj {
    Owner *x0;
    char pad4[0x30 - 0x4];
    Pair x30;
    char pad50[0x70 - 0x50];
    int x70;
    void *x74;
} Obj;

extern char D_0029EA20[];
extern void *func_00119790(void *list, u128 *pos);
extern void *func_00119810(void *list, void *item);
extern void func_0010B2B8(Obj *, void *);
extern void func_0010FDA0(Obj *);

void func_0010E860(Obj *obj)
{
    u128 pos;
    char *src;
    void *p;

    src = obj->x0->x792C;
    /* pos = src[0x10] + src[0x50] * 3 */
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(pos) : "r"(src + 0x10), "r"(src + 0x50), "f"(3.0f) : "$2");
    p = func_00119790(D_0029EA20, &pos);
    obj->x74 = p;
    p = func_00119810(D_0029EA20, p);
    if (p) {
        obj->x30 = *(Pair *)p;
        obj->x70 = 0;
    }
    func_0010B2B8(obj, obj->x74);
    func_0010FDA0(obj);
}
