/* UNIT 0x001C3488 0x001C3550 */
typedef struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20[5];
    int x34;
    int x38;
    int x3C;
    int x40;
    int x44;
} Obj;

extern int func_00202DB0(int, int);
extern int func_00222B08(int, int);
extern int func_00238F48(void);

void func_001C3488(Obj *obj, int a, int b, int c, int d, int e)
{
    int i;

    obj->x0 = a;
    obj->x4 = b;
    obj->xC = func_00202DB0(7, c);
    obj->x10 = func_00202DB0(7, d);
    obj->x8 = 1;
    obj->x14 = e;
    obj->x18 = 0;
    obj->x1C = 0;
    for (i = 4; i >= 0; i--) {
        obj->x20[i] = 0;
    }
    obj->x34 = 0;
    obj->x3C = 6;
    obj->x38 = func_00238F48();
    obj->x40 = func_00222B08(0, 6);
    obj->x44 = 5;
}
