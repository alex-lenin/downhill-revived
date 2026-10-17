/* UNIT 0x0023E018 0x0023E030 */
typedef struct Obj {
    char pad0[0xC];
    int xC;
    int x10;
} Obj;

int func_0023E018(Obj *obj) {
    return obj->xC == obj->x10;
}
