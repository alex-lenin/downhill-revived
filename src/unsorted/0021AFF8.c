/* UNIT 0x0021AFF8 0x0021B020 */
struct State { int x0; int x4; };
int func_0021AFF8(struct State *state) {
    int result = 1;
    state = (struct State *)((char *)state + state->x4 * 16);
    if ((*(unsigned long *)((char *)state + 8) & 0xFF0000000000UL) == 0) result = 0;
    return result;
}
