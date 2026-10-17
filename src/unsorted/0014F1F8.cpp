/* UNIT 0x0014F1F8 0x0014F240 */
struct Value { char pad[0x798C]; int x798C; };
struct Global { char pad[0x50130]; Value *x50130; };
struct Obj { char pad[0x50C]; int x50C; };
struct Receiver { void func_001C17B8(int, int); };
struct Container { char pad[0x5B74]; Receiver x5B74; Receiver *Get() { return &x5B74; } };
extern Global D_002AF2C0;
extern Container D_00398C40;
void func_0014F1F8(Obj *obj, int value) {
    obj->x50C = value;
    if (value) {
        D_00398C40.Get()->func_001C17B8(D_002AF2C0.x50130->x798C, 1);
    }
}
