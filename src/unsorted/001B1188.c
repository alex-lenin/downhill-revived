/* UNIT 0x001B1188 0x001B11C8 */
typedef struct {
    char pad0[8]; int x8, xC, x10, x14, x18, x1C;
    char pad20[0x30]; int x50, x54; char pad58[4]; int x5C, x60, x64, x68, x6C;
    char pad70[0x90]; char x100;
} Obj;
void func_001B1188(Obj *obj) {
    obj->x100 = 1;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x1C = 0;
    obj->x18 = 0;
    obj->x14 = 0;
    obj->x54 = 0;
    obj->x50 = 0;
    obj->x5C = 0;
    obj->x60 = 0;
    obj->x6C = 0;
    obj->x64 = 0;
    obj->x68 = 0;
    obj->x10 = 0;
}
