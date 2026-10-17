/* UNIT 0x001107D8 0x00110870 */
typedef struct { char pad[0x14]; unsigned short flags; } Link;
typedef struct { char pad[0x4A4]; short x4A4; } Sub;
typedef struct { char pad[0x70]; float x70; char pad2[0x54]; int xC8; } State;
typedef struct { char *x0; char pad[0x6C]; Link *x70, *x74; char pad2[0x244]; float x2BC; char pad3[0x14]; State x2D4; } Obj;
extern void func_00110870(Obj *, int);
void func_001107D8(Obj *obj) {
    Sub *sub = (Sub *)(obj->x0 + 0x40C0);
    State *state;
    if (sub) {
        state = &obj->x2D4;
        if (state->xC8 < sub->x4A4 && state->x70 < obj->x2BC &&
            (!obj->x70 || !((obj->x70->flags >> 11)&1)) &&
            (!obj->x74 || !((obj->x74->flags >> 11)&1)))
            func_00110870(obj, 0);
    }
}
