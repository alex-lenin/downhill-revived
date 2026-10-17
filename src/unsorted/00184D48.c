/* UNIT 0x00184D48 0x00184D58 */
typedef struct Obj {
    char pad0[0x8];
    int x8;
    int xC;
    int x10;
} Obj;

void func_00184D48(Obj *obj, int a, int b, int c) {
    obj->x8 = a;
    obj->xC = b;
    obj->x10 = c;
}
