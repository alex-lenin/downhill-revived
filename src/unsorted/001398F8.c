/* UNIT 0x001398F8 0x00139940 */
typedef struct Buffer { int x0; char *x4; } Buffer;
typedef struct Part { char pad[0x3F10]; int x3F10; } Part;
typedef struct State { char pad[0x28000]; Part x28000; char pad2BF14[0x316E4-0x2BF14]; Buffer *x316E4; char pad316E8[0x3171C-0x316E8]; int x3171C, x31720; } State;
void func_001398F8(State *state) {
    Buffer *buffer;
    state->x3171C = 0;
    state->x31720 = 0;
    buffer = state->x316E4;
    buffer->x4 = (char *)buffer + (state->x28000.x3F10 * 4 + 0x20);
    state->x316E4->x4 -= 0x10;
}
