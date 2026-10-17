/* UNIT 0x0018D8A8 0x0018D8B8 */
typedef struct Obj {
    char pad0[0x4];
    short x4;
    char pad6[0x2];
    int x8;
    int xC;
} Obj;

void func_0018D8A8(Obj *obj, int a) {
    obj->x4 = 0;
    obj->xC = 0;
    obj->x8 = a;
}
