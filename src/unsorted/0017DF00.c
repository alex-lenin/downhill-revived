/* UNIT 0x0017DF00 0x0017DFA8 */
struct Obj {
    char pad0[0x19C]; int x19C;
    char pad1A0[0x1E4 - 0x1A0]; int x1E4;
    char pad1E8[0x214 - 0x1E8]; int x214; int x218; int x21C; int x220;
    char pad224[0x248 - 0x224]; int x248;
    char pad24C[0x2AC - 0x24C]; int x2AC;
    char pad2B0[0x310 - 0x2B0]; int x310;
    char pad314[0x374 - 0x314]; int x374;
    char pad378[0x41D - 0x378]; char x41D;
};
extern void func_0017EB00(struct Obj *, int, int);
void func_0017DF00(struct Obj *obj, int value) {
    func_0017EB00(obj, 1, value);
    func_0017EB00(obj, 2, value);
    if (obj->x19C != 0) {
        if (obj->x21C != 0 || obj->x220 != 0 || obj->x218 != 0 || obj->x214 != 0) {
            obj->x1E4 = 0;
            obj->x248 = 0;
            obj->x2AC = 0;
            obj->x310 = 31;
            obj->x374 = 61;
            obj->x41D = 1;
        }
    }
}
