/* UNIT 0x001E7E80 0x001E8298 */
#include "rider.h"
struct Entry { int x0, x4, x8, xC, x10; float x14; char p18[0xC]; float x24; char p28[0x2C]; };
struct Entry2 { char p0[0x1C]; float x1C; char p20[8]; };
static __inline__ AnimationHandle *PART_HANDLE(RiderComponent *s, int index) { return (AnimationHandle *)((char *)s + (index * 16 + 0x14)); }
struct Anim {
    Rider *x0;
    char p4[8];
    int xC;
    unsigned char x10;
    char p11[0x17];
    int x28, x2C, x30, x34;
    char p38[0x194];
    int x1CC;
    char p1D0[0x10];
    float x1E0, x1E4;
    int x1E8, x1EC;
    unsigned char x1F0, x1F1, x1F2, x1F3, x1F4, x1F5, x1F6;
    char p1F7[0x11];
    AnimationHandle x208;
    int x218;
};
extern struct Entry D_00272888[];
extern struct Entry2 D_002736A8[];
extern void func_00136030(void *, int);
extern void func_001E5990(RiderSub6610 *, int);
extern float func_001E5198(RiderSub6610 *);




extern void func_001AD580(void *, int, float, float);
extern void func_001E9F10(struct Anim *, int, float);
extern void func_001E9AF8(struct Anim *, float);
extern void func_001AE1D0(void *, float);
extern void func_00218498(int, int);

void func_001E7E80(struct Anim *a)
{
    AnimationHandle h;
    Rider *r;
    RiderSub6610 *sub;
    RiderComponent *s;
    struct Entry *e;
    struct Entry2 *e2;
    float scale, factor, volume, amount;

    a->xC = 0;
    r = a->x0;
    a->x1E8 = 0;
    a->x1EC = 0;
    a->x10 = 0;
    a->x1F0 = 0;
    a->x1F1 = 0;
    a->x1F2 = 0;
    a->x30 = 0;
    a->x1F3 = 0;
    a->x1F4 = 0;
    a->x1F6 = 0;
    a->x1CC = 1;
    sub = &r->sub6610;
    s = &r->component;
    a->x28 = a->x2C;
    func_00136030(&r->physics, 1);
    func_001E5990(sub, sub->x4);
    sub->x10E6 = 0;
    e = &D_00272888[a->x2C];
    e2 = &D_002736A8[a->x2C];
    a->x34 = D_00272888[a->x2C].x4;
    if ((unsigned int)(a->x2C - 0x21) < 2 && r->x7908 >= 0x23) {
        a->x34 = 0xE0;
    }
    scale = e->x14 * func_001E5198(sub) * s->x211C;
    factor = e->x24 * 0.01f;
    h = *PART_HANDLE(s, a->x34);
    volume = func_00204280(&h);
    amount = factor * volume * func_001E5198(sub);
    if (a->x1F5) {
        amount += amount;
    }
    amount *= 16.66667f;
    if (s->x2160) {
        h = *PART_HANDLE(s, a->x34);
        func_00204080(&h, 1);
        h = *PART_HANDLE(s, a->x34);
        func_00203E10(&h, scale);
        func_00202E38(PART_HANDLE(s, a->x34), 1, 1, amount);
        if (e2->x1C > 0.0f) {
            func_001AD580((char *)r + 0x4580, 0, e2->x1C, 0.5f);
        }
        func_001E9F10(a, 0, amount);
        func_001E9AF8(a, amount);
        if (e->x10 && a->x34) {
            h = r->handles[a->x34];
            func_00204080(&h, 1);
            h = r->handles[a->x34];
            func_00203E10(&h, scale);
            func_00202E38(&r->handles[a->x34], 1, 1, amount);
            func_001AE1D0(r->x48B0, amount);
        }
        if (a->x34 == 0xFD) {
            func_00218498(a->x218, 1);
            h = a->x208;
            func_00203E10(&h, scale);
            func_00202E38(&a->x208, 1, 1, amount);
        }
    } else if (a->x34) {
        h = *PART_HANDLE(s, a->x34);
        func_00203E10(&h, scale);
        func_00202E38(PART_HANDLE(s, a->x34), 1, 1, 30.0f);
    }
    a->x1E0 = 0.5f;
    a->x1E4 = 0.5f;
}
