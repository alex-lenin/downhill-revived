/* UNIT 0x00153118 0x00153168 */
typedef struct Obj {
    char pad0[8];
    float x8, xC, x10;
    char pad14[0xC];
    unsigned char x20, x21;
} Obj;
extern void func_001524B0(Obj *, int);
void func_00153118(Obj *obj, int value) {
    func_001524B0(obj, value);
    obj->x8 = 1.5f;
    obj->xC = 50.0f;
    obj->x10 = 50.0f;
    obj->x20 = 0;
    obj->x21 = 1;
}
