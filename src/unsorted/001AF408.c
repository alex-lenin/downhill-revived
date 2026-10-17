/* UNIT 0x001AF408 0x001AF438 */
typedef struct Obj { int x0, x4, x8, xC; float x10; int x14; } Obj;
void func_001AF408(Obj *obj, int arg1) {
    if (arg1) obj->x14 = arg1;
    obj->x10 = 0.3f;
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x0 = 0;
}
