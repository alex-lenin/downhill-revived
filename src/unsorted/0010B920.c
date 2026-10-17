/* UNIT 0x0010B920 0x0010B998 */
struct Obj { char pad[0x14]; unsigned short flags; };
struct A {
    char pad0[0x74];
    struct Obj *x74, *x78, *x7c;
    char pad80[0x240 - 0x80];
    float x240;
    char pad244[0x258 - 0x244];
    float x258;
    char pad25c[0x380 - 0x25c];
    int x380;
};

void func_0010B920(struct A *a) {
    if (a->x7c == 0 && a->x380 < 7) {
        if ((a->x74->flags >> 14) & 1)
            a->x7c = a->x74;
        else if (a->x258 - a->x240 < 50.0f && ((a->x78->flags >> 14) & 1))
            a->x7c = a->x78;
    }
}
