/* UNIT 0x00222330 0x002223F0 */
struct Vec3 {
    float x, y, z;
};

/* Approximate distance: max + 11/32 * mid + 1/4 * min */
float func_00222330(struct Vec3 *a, struct Vec3 *b)
{
    float dx, dy, dz, t;

    if (b == 0) {
        dx = __builtin_fabsf(a->x);
        dy = __builtin_fabsf(a->y);
        dz = __builtin_fabsf(a->z);
    } else {
        dx = __builtin_fabsf(b->x - a->x);
        dy = __builtin_fabsf(b->y - a->y);
        dz = __builtin_fabsf(b->z - a->z);
    }
    if (dx < dy) {
        t = dx;
        dx = dy;
        dy = t;
    }
    if (dx < dz) {
        t = dx;
        dx = dz;
        dz = t;
    }
    if (dy < dz) {
        t = dy;
        dy = dz;
        dz = t;
    }
    return dx + dy * 0.34375f + dz * 0.25f;
}
