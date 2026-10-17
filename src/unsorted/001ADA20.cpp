/* UNIT 0x001ADA20 0x001ADAC8 */
/* min.s comes from g++'s minimum operator `<?` (MIN_EXPR), not inline asm. */
struct Obj {
    char pad0[0x8];
    float x8;
    char padC[0x10];
    float x1C;
    float x20;
    char pad24[0xD4];
    char *xF8;
};

extern "C" float func_001385A8(void *p);

extern "C" float func_001ADA20(Obj *o) {
    float first, second, result;

    if (o->xF8) {
        first = o->x8;
        first *= 1.0f - (func_001385A8(o->xF8 + 0x40C0) <? 1.0f);
        second = o->x8;
        second *= (func_001385A8(o->xF8 + 0x40C0) + 0.3f) <? 1.0f;
        result = first * o->x1C + second * o->x20;
    } else {
        result = o->x8;
    }
    return result;
}
