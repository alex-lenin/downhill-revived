/* UNIT 0x0012D628 0x0012D660 */
typedef struct Position { char pad[0x20]; float x20, x24, x28; } Position;
typedef struct Part { char pad[0x16D4]; Position *x16D4; } Part;
typedef struct State { char pad0[0x1E40]; Part x1E40; char pad3518[0x7710-0x3518]; float x7710, x7714, x7718, x771C; char pad7720[0x7A8C-0x7720]; float x7A8C; } State;
void func_0012D628(State *state) {
    Part *part;
    part = &state->x1E40;
    state->x7710 = part->x16D4->x20;
    state->x7714 = part->x16D4->x24;
    state->x7718 = part->x16D4->x28;
    state->x771C = state->x7A8C;
}
