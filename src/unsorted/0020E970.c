/* UNIT 0x0020E970 0x0020E990 */
typedef struct { float x0, x4, x8, xC; } Obj;
void func_0020E970(Obj *obj, float a, float b, float c) {
    obj->x0 = b;
    obj->x4 = c;
    obj->x8 = 1.0f;
    obj->xC = a;
}
