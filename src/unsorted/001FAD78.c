/* UNIT 0x001FAD78 0x001FADD0 */
struct State { unsigned char pad0[0x4414]; unsigned char x4414; unsigned char pad4415[0x7F]; unsigned char x4494; };
int func_001FAD78(struct State **obj, int mode) {
    int result1, result2;
    unsigned char value;
    struct State *state = *obj;
    if (mode == 0) {
        result2 = 0;
        if (state->x4494 == 2 || state->x4414 != 2) result2 = 1;
        return result2;
    }
    value = state->x4494;
    result1 = 0;
    if (value != 2 || state->x4414 == value) result1 = 1;
    return result1;
}
