/* UNIT 0x001359F8 0x00135A30 */
struct Value { int x0; int x4; };
extern Value D_0029C3A8[];
class Obj {
public:
    char pad0[0x4B4];
    Value x4B4;
    void func_00135A30();
    void func_001359F8();
};
void Obj::func_001359F8() {
    x4B4 = D_0029C3A8[0];
    this->func_00135A30();
}



