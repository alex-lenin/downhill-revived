/* UNIT 0x0018E398 0x0018E3A8 */
typedef struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x4B9 - 0x41C];
    char x4B9;
    char pad4BA[0x4F4 - 0x4BA];
    int x4F4;
} Obj;

void func_0018E398(Obj *obj) {
    obj->x418 = 0;
    obj->x4F4 = 0;
    obj->x4B9 = 0;
}
