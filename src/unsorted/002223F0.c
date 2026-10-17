/* UNIT 0x002223F0 0x00222460 */
/* Approximate 2D distance between a and b (or length of a when b is NULL):
   max(|dx|,|dy|) + 0.34375 * min(|dx|,|dy|) */
float func_002223F0(float *a, float *b)
{
    float dx, dy, t;

    if (b == 0) {
        dx = __builtin_fabsf(a[0]);
        dy = __builtin_fabsf(a[1]);
    } else {
        dx = __builtin_fabsf(b[0] - a[0]);
        dy = __builtin_fabsf(b[1] - a[1]);
    }
    if (dx < dy) {
        t = dx;
        dx = dy;
        dy = t;
    }
    return dx + dy * 0.34375f;
}
