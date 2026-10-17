/* UNIT 0x001E4120 0x001E4150 */
typedef struct Obj {
    char pad0[0x510];
    int x510;
    int x514;
    unsigned char x518, x519, x51A, x51B, x51C, x51D, x51E, x51F;
    char pad520[0xC];
    int x52C;
} Obj;
void func_001E4120(Obj *obj) {
    obj->x510 = -1;
    obj->x519 = 0;
    obj->x52C = 0;
    obj->x51A = 0;
    obj->x51B = 0;
    obj->x51C = 0;
    obj->x51D = 0;
    obj->x51E = 0;
    obj->x51F = 0;
    obj->x518 = 0;
}

