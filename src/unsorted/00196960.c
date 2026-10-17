/* UNIT 0x00196960 0x00196970 */
typedef struct Obj {
    char pad0[0x4];
    int x4;
    int *x8;
} Obj;

void func_00196960(Obj *obj, int a, int b) {
    *obj->x8 = b;
    obj->x4 = a;
}
