/* UNIT 0x0012D860 0x0012DDC0 */
#include "object_registry.h"
#include "particle.h"
// VU0 vector operations use the original inline instructions.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Mat { Vec v[4]; } Mat;
typedef struct Contact { int x0; u16 x4; u16 x6; int pad8[2]; Mat x10; } Contact;
typedef struct Sub { unsigned int x0 : 6; char pad4[0xC]; Vec x10; Vec x20; } Sub;
typedef struct V { long pad : 2; unsigned long v : 8; } V;
typedef struct SubF { char pad[0xBC]; int xBC; char padC0[8]; unsigned long xC8; } SubF;
typedef struct Body { unsigned int x0; int pad4; int x8; int xC; Sub *x10; } Body;
typedef struct Ent { int x0; char pad4[2]; u16 x6; } Ent;
typedef struct Effect {
    char pad0[0x2720];
    int x2720;
    char pad2724[2];
    u16 x2726;
    char pad2728[4];
    float x272C;
    char pad2730[0x10];
    float x2740;
    float x2744;
    char pad2748[0x24];
    u16 x276C;
    char pad276E[0xAE];
    u32 x281C;
    char pad2820[0x28];
    void *x2848;
} Effect;
typedef struct Mode { char pad[0x10C]; int x10C; char pad110[0x190]; int x2A0; } Mode;
typedef struct Machine Machine;
struct Machine {
    char pad0[0x3FD0];
    SubF x3FD0;
    char pad40A0[0x20];
    float x40C0[4];
    char pad40D0[0x15C];
    int x422C;
    char pad4230[0xC9C];
    int x4ECC;
    char pad4ED0[0x1410];
    int x62E0;
    char pad62E4[0x3C];
    int x6320;
    char pad6324[0x1608];
    void *x792C;
    char pad7930[0x48];
    int x7978;
    int x797C;
    char pad7980[0xC4];
    void *x7A44;
    char pad7A48[4];
    Effect *x7A4C;
    char pad7A50[0x70];
    float x7AC0;
};
class Target {
public:
    virtual void v0(Machine *, int);
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

extern "C" Mode D_004B25A0;
struct EventMgr { char pad[0x200]; void func_001C73D8(Machine *, int, int, Vec *, Vec *); };
extern EventMgr D_004B2914;
extern "C" char D_0043EF22[];
extern "C" char D_0038DB60[];
extern "C" char D_00398C40[];
extern "C" char D_003C6930[];
extern "C" char D_004C5640[];

extern "C" int func_00236EF8(void *, void *, void *, int);

extern "C" void func_0010A528(int, void *, void *);
extern "C" void func_00161450(int, u16, int);
extern "C" void func_00183678(int, u16, int);
extern "C" void func_001E4530(int);
extern "C" void func_0012FE38(void *, int);
extern "C" float func_001385A8(void *);
extern "C" int func_001D4850(char *, int);
extern "C" void func_0022DDD8(void *);
extern "C" void *func_0022DD88(int);
extern "C" Ent *func_00227DB8(int);

extern "C" void func_0012D860(Machine *arg0, void *a1, void *a2, int a3)
{
    Vec *t;
    Contact contacts[8];
    Vec a, b;
    int enabled = a3;
    Effect **slot;
    Mode *mode;
    Vec *m;
    Contact *c;
    char *settings;
    int i = 0;
    int n = func_00236EF8(a1, a2, contacts, 8);
    if (n > 0) {
        t = &contacts[0].x10.v[3];
        m = contacts[0].x10.v;
        c = contacts;
        slot = &arg0->x7A4C;
        settings = D_00398C40;
        mode = &D_004B25A0;
        while (i < n) {
            Target *handler = (Target *)func_0017D3D0(c->x6);
            if (handler) {
                handler->v0(arg0, 0);
            }
            Body *body = (Body *)c->x0;
            if (body->xC) {
                func_0010A528(body->xC, (int *)arg0->x792C + 0x10, (int *)arg0->x792C + 4);
            }
            if (c->x4 == 0x1F74) {
                SubF *subf = &arg0->x3FD0;
                unsigned char v = (unsigned char)(subf->xC8 >> 2);
                if ((v & 1) == 0 && subf->xBC > 0
                        && arg0->x4ECC != (int)arg0 + 0x6320
                        && arg0->x4ECC != (int)arg0 + 0x62E0) {
                    D_0043EF22[arg0->x797C] = 1;
                }
            }
            if (mode->x10C == 0x10 && (unsigned int)(c->x4 - 0x332C) < 0xC8) {
                func_00161450((int)&D_0038DB60[0], ((volatile Contact *)c)->x4, arg0->x7978);
            }
            if (mode->x10C == 0x11 && (unsigned int)(c->x4 - 0x33F4) < 0x33) {
                func_00183678((int)&D_003C6930[0], ((volatile Contact *)c)->x4, arg0->x7978);
            } else {
                union { float f; int i; } currentMode;
                currentMode.f = *(float *)&mode->x10C;
                if (currentMode.i == 0xB && c->x4 == 0x3430)
                    func_001E4530((int)&D_004C5640[0]);
            }
            if (body->x8 > 0 && ((Sub *)body->x10)->x0 == 7
                        && arg0->x4ECC != (int)arg0 + 0x6320) {
                    func_0012FE38(arg0, (int)arg0 + 0x6320);
            }
            float speed = func_001385A8(arg0->x40C0);
            if (body->x8 > 0 && ((Sub *)body->x10)->x0 == 3 && enabled) {
                __asm__ volatile ("sqc2 $vf0, %0" : "=m" (a));
                __asm__ volatile ("sqc2 $vf0, %0" : "=m" (b));
                if (m) {
                    Sub *o = (Sub *)body->x10;
                    if (o->x10.w == 1.0f) {
                        __asm__ volatile (
                            "lqc2 $vf11, 0(%1)\n\t"
                            "lqc2 $vf12, 0(%2)\n\t"
                            "vsub.xyz $vf11, $vf11, $vf12\n\t"
                            "sqc2 $vf11, %0"
                            : "=m" (a) : "r" (&o->x10), "r" (t), "m" (*t));
                        __asm__ volatile (
                            "lqc2 $vf11, 0(%1)\n\t"
                            "lqc2 $vf12, 0(%2)\n\t"
                            "lqc2 $vf13, 16(%2)\n\t"
                            "lqc2 $vf14, 32(%2)\n\t"
                            "vmulax.xyz $ACC, $vf12, $vf11x\n\t"
                            "vmadday.xyz $ACC, $vf13, $vf11y\n\t"
                            "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                            "sqc2 $vf11, %0"
                            : "+m" (a) : "r" (&a), "r" (m));
                        a.w = 1.0f;
                    }
                    if (o->x20.w == 1.0f) {
                            __asm__ volatile (
                                "lqc2 $vf11, 0(%1)\n\t"
                                "lqc2 $vf12, 0(%2)\n\t"
                                "vsub.xyz $vf11, $vf11, $vf12\n\t"
                                "sqc2 $vf11, %0"
                                : "=m" (b) : "r" (&o->x20), "r" (&c->x10.v[3]));
                            __asm__ volatile (
                                "lqc2 $vf11, 0(%1)\n\t"
                                "lqc2 $vf12, 0(%2)\n\t"
                                "lqc2 $vf13, 16(%2)\n\t"
                                "lqc2 $vf14, 32(%2)\n\t"
                                "vmulax.xyz $ACC, $vf12, $vf11x\n\t"
                                "vmadday.xyz $ACC, $vf13, $vf11y\n\t"
                                "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
                                "sqc2 $vf11, %0"
                                : "+m" (b) : "r" (&b), "r" (m));
                            a.w = 1.0f;
                    }
                }
                EventMgr *manager = &D_004B2914;
                unsigned flags = body->x0;
                manager->func_001C73D8(arg0, flags >> 18, (flags >> 7) & 0x7FF, &a, &b);
                unsigned int masked = body->x0 & ~0x3FFFF;
                float lim;
                if (masked == 0x19340000) goto effect;
                if (masked == 0x19740000 || masked == 0x19780000 || masked == 0x197C0000) {
                    lim = arg0->x7AC0;
                    if ((float)func_001D4850(settings + 0x240, body->x0 >> 18) < lim && speed > 0.008f) {
                    effect:
                        Effect *e = (Effect *)arg0->x7A4C;
                        if (e) {
                            if ((unsigned int)e->x276C < 0x31) {
                                func_0022DDD8(e);
                                e = (Effect *)0;
                                arg0->x7A4C = (Effect *)0;
                            } else if (e->x276C == 0x31) {
                                Ent *ent = func_00227DB8(0x31);
                                e->x2720 = ent->x0 | 4;
                                ent = func_00227DB8(0x31);
                                e->x2726 = ent->x6;
                                e->x272C = speed * 2.0f;
                                e->x2740 = speed * 0.25f;
                                e->x2744 = speed * 0.5f;
                            }
                        }
                        if (!e) {
                            Effect *src = (Effect *)arg0->x7A44;
                            int color = ((src->x281C >> 1) & 0x7F7F7F) | 0x80000000;
                            e = (Effect *)func_0022DD88(func_0022CFB8((Vector4 *)((int *)arg0->x792C + 4), (int *)arg0->x792C + 0x10, 0x31, (Vector4 *)((void *)0), 1.0f, (Matrix4 *)((void *)0), (int)(0), (int)(color)));
                            if (e) {
                                *(Effect *volatile *)&arg0->x7A4C = e;
                                e->x272C = speed * 2.0f;
                                e->x2740 = speed * 0.25f;
                                e->x2744 = speed * 0.5f;
                                e->x2848 = slot;
                            }
                                if (mode->x2A0 == 0x5 || mode->x2A0 == 0x16 || mode->x2A0 == 0xE) {
                                    int color2 = (src->x281C & 0xFFFFFF) | 0xFF000000;
                                    func_0022CFB8((Vector4 *)((int *)arg0->x792C + 4), (int *)arg0->x792C + 0x10, 0x20, (Vector4 *)((void *)0), 5.0f * speed, (Matrix4 *)((void *)0), (int)(0), (int)(color2));
                                }
                        }
                    }
                }
                i++;
            } else {
                i++;
            }
            t += 5;
            m += 5;
            c++;
        }
    }
}
