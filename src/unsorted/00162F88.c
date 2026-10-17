/* UNIT 0x00162F88 0x00162FA8 */
typedef struct Obj { char pad0[8]; int x8; int xC; float x10, x14; } Obj;
void func_00162F88(Obj *obj) {
    obj->x8 = 0;
    obj->x14 = 1.0f;
    obj->x10 = 150.0f;
}
