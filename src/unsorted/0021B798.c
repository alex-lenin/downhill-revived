/* UNIT 0x0021B798 0x0021B7C8 */
typedef struct { char pad0[0xFA0]; int xFA0, xFA4, xFA8; } Obj;
extern Obj D_00620360;
void func_0021B798(int enabled, int value) {
    if (value == 0xFFFF && enabled) {
        D_00620360.xFA4 = 0;
        D_00620360.xFA0 = 1000;
        D_00620360.xFA8 = 0;
    }
}
