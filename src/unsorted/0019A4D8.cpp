/* UNIT 0x0019A4D8 0x0019A530 */
extern int D_003F52F0[];
extern char D_0043DCC0[];
extern void func_0019BBE0(void *, int);
extern void func_0018DC20(void *);
struct A {
 int x0;
 void func_0019A530(int);
 void func_0019A4D8();
};
void A::func_0019A4D8() {
 if (x0 < 4) func_0019A530(x0);
 func_0019BBE0(D_0043DCC0, D_003F52F0[0]);
 func_0018DC20(this);
}

