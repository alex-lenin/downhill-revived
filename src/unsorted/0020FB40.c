/* UNIT 0x0020FB40 0x0020FB80 */
typedef struct { char pad[8]; short x8, xa, xc, xe; char pad10[16]; } Obj;
extern Obj D_004F3570[];
void func_0020FB40(int index, int x, int y, int width, int height) {
    Obj *obj = &D_004F3570[index];
    obj->x8 = x << 4;
    obj->xa = y << 4;
    obj->xe = (x + width) << 4;
    obj->xc = (y + height) << 4;
}
