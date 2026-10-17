/* UNIT 0x0019B000 0x0019B0B8 */
typedef struct State {
    char pad[0x500];
    int x500;
    char pad504[8];
    signed char x50C[4][13];
    signed char x540[4];
} State;

extern struct Global {
    char pad[0x12];
    signed char x12;
    char pad13;
    int x14[4];
} D_0043DCC0;

void func_0019B000(State *state, int index)
{
    int i;

    if (state->x540[state->x500] > 0) {
        D_0043DCC0.x14[D_0043DCC0.x12] = 1;
        for (i = index; i < 12; i++) {
            state->x50C[state->x500][i] = state->x50C[state->x500][i + 1];
        }
        state->x50C[state->x500][12] = 40;
        state->x540[state->x500]--;
    }
}
