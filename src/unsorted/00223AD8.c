/* UNIT 0x00223AD8 0x00223B00 */
struct State { int x0; int x4; int x8; int xC; };
extern void func_00223B00(struct State *);
void func_00223AD8(struct State *state, int start, int size) {
    state->x0 = start;
    state->xC = start + size;
    func_00223B00(state);
}
