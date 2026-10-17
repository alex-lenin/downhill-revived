/* UNIT 0x0012DEB0 0x0012DF48 */
extern float fabsf(float);

struct Obj {
    char pad0[0x1190];
    int x1190;
    int x1194;
    char pad1198[0x1258 - 0x1198];
    int x1258;
    int x125C;
    char pad1260[0x13D7 - 0x1260];
    unsigned char x13D7;
    unsigned char x13D8;
};

float func_0012DEB0(struct Obj *obj) {
    int v;
    float f;

    if (obj->x13D7) v = obj->x1258;
    else v = obj->x1190;
    f = (float)v;
    if (f < 1e-10f) {
        if (obj->x13D8) v = obj->x125C;
        else v = obj->x1194;
        f = (float)-v;
    }
    /* Zero-byte label: emits no bytes; a scheduling barrier only (same device
       as promoted src/unsorted/0012F8D8.c). It pins the join of the branch
       above so the delay-slot pass leaves bc1f's slot empty (retail nop)
       instead of copying abs.s into it as bc1fl. */
    __asm__ volatile ("Lj00:" : : "f" (f));
    if (fabsf(f) > 1.0f) {
        f *= 0.0078125f;
    }
    return f;
}
