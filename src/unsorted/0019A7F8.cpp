/* UNIT 0x0019A7F8 0x0019A850 */
struct State { char pad[0x11]; signed char x11, x12, x13; };
extern State D_0043DCC0;
struct A {
 signed char bytes[0x600];
 void func_0018E468();
 void func_0019A7F8();
};
void A::func_0019A7F8() {
 if (D_0043DCC0.x13 && !D_0043DCC0.x11 && D_0043DCC0.x12 > 0 && bytes[0x53f + D_0043DCC0.x12] == 0)
  D_0043DCC0.x13 = 0;
 this->func_0018E468();
}
