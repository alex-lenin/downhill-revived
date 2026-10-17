/* UNIT 0x001524B8 0x001524D8 */
typedef struct Obj { char pad0[0x14]; int x14; unsigned char x18, x19, x1A; float x1C; } Obj;
void func_001524B8(Obj *obj) {
    obj->x14 = 0;
    obj->x1C = 100.0f;
    obj->x1A = 0;
    obj->x18 = 0;
    obj->x19 = 0;
}
