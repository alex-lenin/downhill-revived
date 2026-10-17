/* UNIT 0x0012DE68 0x0012DEB0 */
typedef struct State { char pad[0x3514]; void *x3514; char pad3518[0x794C-0x3518]; void *x794C; } State;
extern void func_00218498(void *, int);
extern void func_002182A8(void *, int);
void func_0012DE68(State *state, int value) {
    if (state->x794C != 0) {
        func_00218498(state->x794C, value);
    }
    func_002182A8(state->x3514, value == 0);
}
