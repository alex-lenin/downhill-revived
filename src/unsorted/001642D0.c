/* UNIT 0x001642D0 0x00164A10 */
typedef struct { float x, y, z, w; } Vec;
typedef struct {
    float x0, x4, x8, xC, x10, x14, x18, x1C, x20, x24, x28, x2C, x30, x34;
    int x38, x3C, x40;
    char pad44[12];
    float x50[3];
    char pad5C[4];
    Vec x60, x70, x80, x90, xA0;
    char padB0[16];
} Entry;
extern Entry D_003960A0[];
static inline void set3b(Vec *v, float x, float y, float z) { v->x = 0.0f; v->y = x; v->z = y; v->w = z; }
static inline void set4b(Vec *v, float a, float b, float c, float d) { v->x = a; v->y = b; v->z = c; v->w = d; }
static inline void set2(float *p, float x, float y, float z) { p[0] = x; p[1] = y; p[2] = z; }
void func_001642D0(void) {
    Entry *e0 = &D_003960A0[0];
    Entry *e1 = &D_003960A0[1];
    Entry *e2 = &D_003960A0[2];
    Entry *e3 = &D_003960A0[3];
    Entry *e4 = &D_003960A0[4];
    Entry *e5 = &D_003960A0[5];
    Entry *e6 = &D_003960A0[6];
    e0->x0 = 25.0f; e0->x4 = 0.2f; e0->x8 = 0.5f; e0->xC = 0.25f; e0->x10 = 0.987f; e0->x14 = 0.975f; e0->x18 = 3.0f; e0->x1C = 400.0f;
    e0->x20 = 15.0f; e0->x24 = 1.0f; e0->x28 = 0.5f; e0->x2C = 0.3f; e0->x30 = 5.0f; e0->x34 = 0.0f;
    e0->x38 = 180; e0->x3C = 2; e0->x40 = 4;
    set2(e0->x50, 0.0f, 0.0f, 3.5f);
    set3b(&e0->x60, 0.0f, 2.0f, 0.75f);
    set3b(&e0->x70, 0.0f, 5.0f, 0.75f);
    set4b(&e0->x80, -2.0f, 0.0f, 1.5f, 0.0f);
    set4b(&e0->x90, 2.0f, 0.0f, 1.5f, 0.0f);
    set4b(&e0->xA0, 2.0f, 0.0f, 5.5f, 0.0f);
    e1->x0 = 25.0f; e1->x4 = 0.2f; e1->x8 = 0.5f; e1->xC = 0.25f; e1->x10 = 0.98f; e1->x14 = 0.98f; e1->x18 = 1.5f; e1->x1C = 5.0f;
    e1->x20 = 25.0f; e1->x24 = 2.0f; e1->x28 = 0.5f; e1->x2C = 0.5f; e1->x34 = 15.0f; e1->x30 = 7.0f;
    e1->x38 = 120; e1->x3C = 2; e1->x40 = 0;
    set2(e1->x50, 0.0f, 0.0f, 1.361f);
    set3b(&e1->x60, 1.361f, 1.361f, 1.4f);
    set3b(&e1->x70, -1.361f, 1.361f, 1.4f);
    set3b(&e1->x80, 2.0f, 0.0f, 0.0f);
    set3b(&e1->x90, 2.0f, 2.0f, 0.0f);
    set3b(&e1->xA0, -2.0f, 2.0f, 0.0f);
    e2->x0 = 20.0f; e2->x4 = 0.2f; e2->x8 = 0.5f; e2->xC = 0.25f; e2->x10 = 0.98f; e2->x14 = 0.98f; e2->x18 = 2.0f; e2->x1C = 5.0f;
    e2->x20 = 25.0f; e2->x24 = 1.5f; e2->x28 = 0.5f; e2->x2C = 0.5f; e2->x34 = 20.0f; e2->x30 = 10.0f;
    e2->x38 = 120; e2->x3C = 2; e2->x40 = 0;
    set2(e2->x50, 0.0f, 0.0f, 3.306f);
    set3b(&e2->x60, 2.5f, 3.306f, 2.0f);
    set3b(&e2->x70, -2.2f, 3.306f, 2.0f);
    e3->x0 = 10.0f; e3->x4 = 0.4f; e3->x8 = 0.5f; e3->xC = 0.25f; e3->x10 = 0.975f; e3->x14 = 0.975f; e3->x18 = 1.5f; e3->x1C = 5.0f;
    e3->x20 = 0.25f; e3->x24 = 2.0f; e3->x28 = 0.5f; e3->x2C = 0.3f; e3->x34 = 0.0f; e3->x30 = 0.0f;
    e3->x38 = 120; e3->x3C = 2; e3->x40 = 0;
    set2(e3->x50, 0.0f, 0.0f, 3.354f);
    set3b(&e3->x60, 3.354f, 3.354f, 3.005f);
    set3b(&e3->x70, -3.354f, 3.354f, 3.005f);
    e4->x0 = 10.0f; e4->x4 = 0.4f; e4->x8 = 0.5f; e4->xC = 0.25f; e4->x10 = 0.975f; e4->x14 = 0.975f; e4->x18 = 1.5f; e4->x1C = 5.0f;
    e4->x20 = 0.25f; e4->x24 = 2.0f; e4->x28 = 0.5f; e4->x2C = 0.3f; e4->x34 = 0.0f; e4->x30 = 0.0f;
    e4->x38 = 120; e4->x3C = 2; e4->x40 = 0;
    set2(e4->x50, 0.0f, 0.0f, 2.196f);
    set3b(&e4->x60, 2.196f, 2.196f, 2.716f);
    set3b(&e4->x70, -2.196f, 2.196f, 2.716f);
    e5->x0 = 20.0f; e5->x4 = 0.2f; e5->x8 = 0.5f; e5->xC = 0.25f; e5->x10 = 0.98f; e5->x14 = 0.98f; e5->x18 = 1.5f; e5->x1C = 5.0f;
    e5->x20 = 15.0f; e5->x24 = 1.0f; e5->x28 = 0.5f; e5->x2C = 0.5f; e5->x34 = 15.0f; e5->x30 = 7.0f;
    e5->x38 = 120; e5->x3C = 2; e5->x40 = 0;
    set2(e5->x50, 0.0f, 0.4f, 2.361f);
    set3b(&e5->x60, 1.8f, 2.361f, 1.4f);
    set3b(&e5->x70, -1.01f, 2.361f, 1.4f);
    set3b(&e5->x80, 2.0f, 0.0f, 0.0f);
    set3b(&e5->x90, 2.0f, 2.0f, 0.0f);
    set3b(&e5->xA0, -2.0f, 2.0f, 0.0f);
    e6->x0 = 20.0f; e6->x4 = 0.2f; e6->x8 = 0.5f; e6->xC = 0.25f; e6->x10 = 0.95f; e6->x14 = 0.98f; e6->x18 = 1.5f; e6->x1C = 10.0f;
    e6->x20 = 20.0f; e6->x24 = 1.0f; e6->x28 = 0.25f; e6->x2C = 0.5f; e6->x34 = 15.0f; e6->x30 = 7.0f;
    e6->x38 = 1120; e6->x3C = 2; e6->x40 = 0;
    set2(e6->x50, 0.0f, 0.0f, 1.0f);
    set3b(&e6->x60, 1.0f, 1.0f, 1.0f);
    set3b(&e6->x70, -0.5f, 1.0f, 1.0f);
    set3b(&e6->x80, 1.0f, 0.0f, 0.0f);
    set3b(&e6->x90, 1.0f, 1.0f, 0.0f);
    set3b(&e6->xA0, -1.0f, 1.0f, 0.0f);
}
