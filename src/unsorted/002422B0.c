/* UNIT 0x002422B0 0x00242328 */
typedef struct Obj {
    int x0;
    char pad4[0x28];
    int x2C, x30, x34, x38, x3C, x40, x44, x48, x4C, x50, x54;
} Obj;

extern int func_00202A30(int, int, int, int, int, int);

int func_002422B0(Obj *obj, int a, int b, int c) {
    int result;
    obj->x30 = a;
    obj->x3C = b;
    obj->x0 = 0;
    obj->x2C = 0;
    obj->x34 = 0;
    obj->x38 = 0;
    obj->x40 = 0;
    obj->x54 = 0;
    obj->x4C = 0;
    obj->x50 = 0;
    obj->x48 = c;
    result = func_00202A30(c, 0, 0x400, 0, 6, 2);
    obj->x44 = result;
    if (result < 0) {
        return 0;
    }
    return 1;
}
