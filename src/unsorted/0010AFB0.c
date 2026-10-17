/* UNIT 0x0010AFB0 0x0010AFE0 */
struct A { char pad0[0x288]; float x288; char pad28c[0x3c0 - 0x28c]; int x3c0; };

void func_0010AFB0(struct A *a, int on) {
    a->x3c0 = on;
    if (on) a->x288 = 0.3f;
    else a->x288 = 0.4f;
}
