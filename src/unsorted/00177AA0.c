/* UNIT 0x00177AA0 0x00177AC8 */
typedef struct Obj {
    char pad0[0x136C];
    int x136C;
    int x1370;
} Obj;

extern int D_004B2828[];

void func_00177AA0(Obj *obj, int value)
{
    if (D_004B2828[0] == 1) {
        obj->x136C = value;
        obj->x1370 = 300;
    }
}
