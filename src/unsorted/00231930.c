/* UNIT 0x00231930 0x00231968 */
typedef struct Vec {
    float x, y, z, w;
} Vec;

typedef struct Obj {
    char pad0[0x70];
    Vec x70;
    char pad80[0xD0 - 0x80];
    Vec xD0;
} Obj;

void func_00231930(Obj *obj, float scale) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(obj->x70) : "r"(&obj->x70), "f"(scale) : "$2");
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0"
                      : "=m"(obj->xD0) : "r"(&obj->xD0), "f"(scale) : "$2");
}
