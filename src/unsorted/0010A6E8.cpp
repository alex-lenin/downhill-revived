/* UNIT 0x0010A6E8 0x0010A730 */
struct B {
    unsigned short x0;
    void func_0010A468();   /* body ignores `this`: see func_0010A468 in 0010A388.c */
};

struct A {
    B *x0;
    char pad4[0x5c];
    unsigned char x60;
    int func_0010A6E8();
};

int A::func_0010A6E8() {
    if (x0->x0 == 9) x0->func_0010A468();
    x60 = 0;
    return 0;
}
