/* UNIT 0x0011D198 0x0011D270 */
extern "C" void func_0011E9F0(struct A *a);

struct A {
    char pad0[0x268];
    unsigned long x268_0 : 49;
    unsigned long x268_49 : 1;
    unsigned long x268_50 : 14;
    int func_0011D148();
    void SetFlag49(int on) { x268_49 = on; }
};

struct B {
    A items[30];
    char pad4920[0x4A10 - 0x4920];
    int x4A10;
    int x4A14;
    char pad4A18[0x4A90 - 0x4A18];
    int x4A90;
    void func_0011CA78();
    void func_0011D198();
};

void B::func_0011D198() {
    int i;
    A *a;

    if (x4A10) {
        if (x4A14) {
            func_0011CA78();
            x4A14 = 0;
        } else {
            for (i = 0; i < x4A90; i++) {
                a = &items[i];
                if (a->func_0011D148()) {
                    a->SetFlag49(1);
                } else {
                    a->SetFlag49(0);
                }
                func_0011E9F0(a);
            }
        }
    }
}
