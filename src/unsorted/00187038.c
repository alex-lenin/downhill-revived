/* UNIT 0x00187038 0x00187048 */
typedef struct Obj {
    char pad0[0x6C];
    int x6C;
    int x70;
} Obj;

void func_00187038(Obj *obj) {
    obj->x70 = 0;
    obj->x6C = 0;
}
