/* UNIT 0x00231A08 0x00231A48 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x34];
    Vec4 *pos;      /* 0x34 */
    char pad38[0x18];
    Vec4 x50;       /* 0x50 */
} Obj;

/* Moves *obj->pos to *newPos, keeping x50 at the same offset from it. */
void func_00231A08(Obj *obj, Vec4 *newPos)
{
    Vec4 offset;

    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(offset) : "r"(&obj->x50), "r"(obj->pos));
    *obj->pos = *newPos;
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, %2\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(obj->x50) : "r"(obj->pos), "m"(offset));
}
