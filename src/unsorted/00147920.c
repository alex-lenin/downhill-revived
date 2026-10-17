/* UNIT 0x00147920 0x00147958 */
typedef struct Obj { int x0; int *x4; int *x8; } Obj;
void func_00147920(Obj *obj) {
    *obj->x4 &= (int)0xFFFC007F;
    *obj->x8 = (*obj->x8 & ~0x3F) | 6;
}
