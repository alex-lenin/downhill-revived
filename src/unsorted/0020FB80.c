/* UNIT 0x0020FB80 0x0020FBB0 */
typedef struct { char pad0[8]; short x8, xA, xC, xE; char pad10[16]; } Obj;
extern Obj D_004F3570[];
void func_0020FB80(int index, int x, int y, int width, int height) {
    Obj *obj = &D_004F3570[index];
    obj->x8 = x;
    obj->xA = y;
    obj->xE = x + width;
    obj->xC = y + height;
}
