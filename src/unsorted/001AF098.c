/* UNIT 0x001AF098 0x001AF0C0 */
typedef struct Obj { int x0; int x4; int x8; float xC; char pad10[8]; void *x18; } Obj;
void func_001AF098(Obj *obj, void *value) {
    obj->x0 = 0;
    if (value) obj->x18 = value;
    obj->x8 = 0;
    obj->xC = 1.0f;
    obj->x4 = 0;
}
