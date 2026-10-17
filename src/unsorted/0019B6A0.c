/* UNIT 0x0019B6A0 0x0019B740 */
typedef struct { char pad[0x504]; int x504, x508; } Obj;
typedef struct { char pad[0x500]; int x500; char pad504[8]; char x50C[1]; } State;
typedef struct { char pad[0x12]; signed char x12; char pad13; int x14[1]; } Flags;
extern State D_0043CD50;
extern Flags D_0043DCC0;
extern void func_0019AF60(State *, int, int);
void func_0019B6A0(Obj *obj, int value) {
    int mode = obj->x508;
    if (mode == 0) {
        func_0019AF60(&D_0043CD50, obj->x504, value);
        D_0043DCC0.x14[D_0043DCC0.x12] = 1;
    } else if (mode == 1) {
        int index = obj->x504 + D_0043CD50.x500 * 13;
        D_0043DCC0.x14[D_0043DCC0.x12] = mode;
        D_0043CD50.x50C[index] = value;
    }
}
