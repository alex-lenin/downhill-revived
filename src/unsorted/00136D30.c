/* UNIT 0x00136D30 0x00136D68 */
typedef struct State {
    char pad0[0x11CC]; int x11CC;
    char pad11D0[0xC4]; int x1294;
    char pad1298[0x14E]; unsigned char x13E6;
} State;
typedef struct Obj {
    char pad0[0x158]; State *x158;
    char pad15C[0x34E]; short x4AA;
    char pad4AC[4]; long x4B0;
} Obj;
void func_00136D30(Obj *obj) {
    State *state = obj->x158;
    int value;
    if (state->x13E6) value = state->x1294;
    else value = state->x11CC;
    if (value == 0 && !(obj->x4B0 & 8)) obj->x4AA = 1;
}

