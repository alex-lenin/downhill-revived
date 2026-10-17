/* UNIT 0x0010C5D8 0x0010C630 */
struct Other;
struct State { char pad0[0x798C]; int x798C; };
class Obj {
public:
    State *x0;
    char pad4[0x39C];
    int x3A0;
    void func_0010C630(Other *other);
    void func_0010C940(Other *other);
    void func_0010C5D8(Other *other);
};
void Obj::func_0010C5D8(Other *other) {
    if (((x3A0 >> 4) & 1) && x0->x798C >= 2) {
        func_0010C940(other);
    } else {
        func_0010C630(other);
    }
}
