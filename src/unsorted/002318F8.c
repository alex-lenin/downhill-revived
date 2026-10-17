/* UNIT 0x002318F8 0x00231930 */
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Obj {
    char pad0[0x60];
    Vec x60;
    char pad70[0xC0 - 0x70];
    Vec xC0;
} Obj;

void func_002318F8(Obj *obj, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(obj->x60) : "r"(&obj->x60), "f"(scale) : "$2");
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(obj->xC0) : "r"(&obj->xC0), "f"(scale) : "$2");
}
