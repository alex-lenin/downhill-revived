/* UNIT 0x001ADAC8 0x001ADBC0 */
/* min.s/max.s come from g++'s `<?`/`>?` operators (MIN_EXPR/MAX_EXPR), as in func_001ADA20. */
struct Ctrl {
    char pad0[0x4];
    float x4;
    float x8;
    char padC[0x8];
    float x14;
    float x18;
    char pad1C[0xDC];
    char *xF8;
};

extern "C" float func_001385A8(void *p);

extern "C" float func_001ADAC8(Ctrl *o) {
    float t, t2, scale, first, second;

    if (!o->xF8) {
        return o->x8;
    }
    t = ((o->x4 >? -0.5f) <? 0.5f) + 0.5f;
    t2 = t * t;
    scale = 1.0f - t2 * ((t2 - t - t + 1.0f) * 16.0f);
    scale *= 0.5f;
    if (o->x4 < 0.0f) {
        scale = -scale;
    }
    first = scale;
    first *= 1.0f - (func_001385A8(o->xF8 + 0x40C0) <? 1.0f);
    second = scale;
    second *= (func_001385A8(o->xF8 + 0x40C0) + 0.3f) <? 1.0f;
    return first * o->x14 + second * o->x18;
}
