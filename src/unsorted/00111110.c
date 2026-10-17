/* UNIT 0x00111110 0x00111150 */
typedef struct Obj { char pad0[0x2BC]; float x2BC; char pad2C0[0xE0]; int x3A0; } Obj;
int func_00111110(Obj *p) {
    int result = 0;
    if (((p->x3A0 >> 5) & 1) && p->x2BC > 0.5f) result = 1;
    return result;
}
