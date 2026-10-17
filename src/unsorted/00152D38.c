/* UNIT 0x00152D38 0x00152D88 */
typedef struct Obj {
    char pad0[8];
    float x8, xC, x10;
    char pad14[0xC];
    unsigned char x20, x21;
} Obj;
extern void func_001524B0(Obj *, int);
void func_00152D38(Obj *obj, int value) {
    func_001524B0(obj, value);
    obj->x8 = 1.3f;
    obj->xC = 100.0f;
    obj->x10 = 100.0f;
    obj->x20 = 0;
    obj->x21 = 1;
}
