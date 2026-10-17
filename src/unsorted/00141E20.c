/* UNIT 0x00141E20 0x00141F58 */
typedef struct Vec4 { float x, y, z, w; } Vec4;

typedef struct Obj {
    char pad0[0x10];
    Vec4 vec[16];   /* xyz = direction, w = distance */
    Vec4 result;    /* 0x110 */
} Obj;

/* lhs + src * scale (xyz) -> dst, VU0 macro mode */
#define add_scaled(dst, lhs, src, scale) \
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\t" \
                      "lqc2 $vf12, 0x0(%1)\n\t" \
                      "mfc1 $2, %2\n\t" \
                      "qmtc2.ni $2, $vf13\n\t" \
                      "vadda.xyz ACC, $vf11, $vf0\n\t" \
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t" \
                      "sqc2 $vf11, 0x0(%3)" \
                      : : "r"(lhs), "r"(src), "f"(scale), "r"(dst) : "$2", "memory")

void func_00141E20(Obj *obj) {
    Vec4 *near, *far, *result;
    Vec4 *scan;
    int i;
    float dist, scale;

    obj->result.x = obj->result.y = obj->result.z = 0.0f;
    near = obj->vec;
    far = obj->vec;
    result = &obj->result;
    scan = (Vec4 *)obj;
    for (i = 0; i < 4; i++) {
        dist = scan[1].w;
        if (dist < 499999.0f) {
            if (dist < 8.0f) {
                scale = (dist - 8.0f) * 0.125f;
                if (dist < 0.000001f) {
                    obj->result.z -= 1.0f;
                } else {
                    scale /= dist;
                    add_scaled(result, result, near, scale);
                }
            } else {
                scale = (dist - 8.0f) * 0.001953125f;
                if (scale > 1.0f) scale = 1.0f;
                scale /= dist;
                add_scaled(result, result, far, scale);
            }
        }
        far++;
        near++;
        scan++;
    }
    /* dead store: leaves an empty block after the loop, which stops sched1
       from hoisting the 8.0f load above the first compare (as in retail) */
    i = 0;
}
