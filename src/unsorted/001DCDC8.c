/* UNIT 0x001DCDC8 0x001DCE20 */
struct V { int x0; float x4; int x8; };
struct Obj { char pad0[0x20]; int x20; float x24; int x28; char pad2c[4]; char pad30[0x10]; float x40; int x44; int x48; int x4c; int x50; float x54; int x58; int x5c; int x60; };
void func_001DCDC8(struct Obj *o) {
    struct V *v;
    __asm__ volatile("sqc2 $vf0, 0x0(%0)\n\tsqc2 $vf0, 0x10(%0)" : : "r"(o) : "memory");
    v = (struct V *)((char *)o + 0x20);
    o->x20 = 0;
    v->x4 = 1.0f;
    v->x8 = 0;
    __asm__ volatile("sqc2 $vf0, 0x30(%0)" : : "r"(o) : "memory");
    o->x40 = 1.0f;
    o->x54 = 0.016666668f;
    o->x44 = 0;
    o->x48 = 0;
    o->x4c = 0;
    o->x50 = 0;
    o->x58 = 0;
    o->x5c = 0;
    o->x60 = 0;
}

