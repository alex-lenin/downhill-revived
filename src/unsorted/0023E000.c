/* UNIT 0x0023E000 0x0023E018 */
typedef struct Obj {
    char pad0[0x8];
    volatile int x8;
    volatile int xC;
} Obj;

int func_0023E000(Obj *obj) {
    obj->xC = 0;
    obj->x8 = 0;
    return 1;
}
