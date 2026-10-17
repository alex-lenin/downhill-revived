/* UNIT 0x001E1BE8 0x001E1C70 */
struct Obj { char pad[0x18]; int x18; char pad1C[0x14]; int x30,x34,x38; };
struct Global { char pad[0x770C]; int x770C; };
extern char D_004225A0[];
extern int D_004B2840[];
extern void func_001E1C70(struct Obj *);
void func_001E1BE8(struct Obj *object) {
    if (object->x38 == 1) {
        func_001E1C70(object);
        object->x30 = object->x18;
        object->x34 = ((struct Global *)(D_004B2840[0] * 0xD8 + D_004225A0))->x770C;
    } else {
        object->x30 = ((struct Global *)(D_004B2840[0] * 0xD8 + D_004225A0))->x770C;
    }
}
