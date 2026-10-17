/* UNIT 0x0018D850 0x0018D860 */
typedef struct Obj {
    char pad0[0x10];
    int x10;
    int x14;
    int x18;
} Obj;

void func_0018D850(Obj *obj, int a, int b, int c) {
    obj->x10 = a;
    obj->x14 = b;
    obj->x18 = c;
}
