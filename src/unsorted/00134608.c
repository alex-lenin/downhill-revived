/* UNIT 0x00134608 0x00134640 */
typedef struct Obj { char pad0[8]; unsigned int **x8; char padC[0x1D4]; unsigned char x1E0; } Obj;
void func_00134608(Obj *obj) {
    unsigned int **p = obj->x8;
    if (p) {
        **p &= 0xFFFC007F;
        obj->x8 = 0;
    }
    obj->x1E0 = 0;
}
