/* UNIT 0x00126DB8 0x00126E08 */
typedef struct Rider {
    char pad0[0x4180];
    float x4180[4];
} Rider;

typedef struct Obj {
    char pad0[0x148];
    Rider *x148;
} Obj;

void func_00126DB8(Obj *obj, float *out)
{
    Rider *rider = obj->x148;

    if (rider != 0) {
        __asm__ volatile ("lq $2, 0x0(%0)\n\tsq $2, 0x0(%1)" : : "r"(rider->x4180), "r"(out) : "$2", "memory");
        __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %1\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%0)" : : "r"(out), "f"(2.0f) : "$2", "memory");
    } else {
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 0.0f;
        out[3] = 0.0f;
    }
}
