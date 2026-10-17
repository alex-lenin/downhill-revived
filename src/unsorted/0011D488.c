/* UNIT 0x0011D488 0x0011E4B8 */
#include "object_registry.h"
#include "scene.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

typedef union Hdr {
    unsigned int word;
    struct {
        unsigned int x0 : 7;
        unsigned int index : 11;
        int id : 14;
    } f;
} Hdr;

typedef SceneNode Model;

typedef struct { int x0, x4, x8, xC; } Sound;
typedef struct { void *x0; void *x4; short x8, xA; } Part;
typedef struct { char pad0[0x48]; float x48; float x4C; } Sub110;
typedef struct { char pad0[0x30]; float x30; float x34; float x38; float x3C; } Sub160;

typedef struct Obj {
    char pad0[4];
    int x4;
    char pad8[8];
    Model *x10;
    Model *x14;
    char pad18[0x18];
    void *x30;
    char pad34[4];
    int x38;
    Sound snd[12];
    Part xFC;
    char pad108[8];
    Sub110 x110;
    Sub160 x160;
    void *x1A0;
    float x1A4;
    char x1A8[0x58];
    float x200;
    float x204;
    float x208;
    float x20C;
    char pad210[0x14];
    float x224;
    float x228;
    char pad22C[4];
    float x230;
    float x234;
    char pad238[0x18];
    float x250;
    float x254;
    float x258;
    char pad25C[6];
    short x262;
    char pad264[4];
    short x268;
    short x26A;
} Obj;

extern int func_00222BD8(int, int);
extern void *func_00163D40(void *, int);
extern void func_0022DDD8(void *);
extern void func_001CFA00(void *, void *);
extern void func_0011E568(Obj *, int);

extern void func_00203590(Sound *, int, int, int);
extern float func_00222B68(float, float);
extern int func_00222B08(int, int);
extern void func_0011E860(Obj *);
extern char D_003041B0[];
extern float D_0029C164[];
extern int D_004B2840[];
extern struct { char pad[0x2A0]; int x2A0; } D_004B25A0;

static inline void InitSub110(Sub110 *s)
{
    s->x48 = 0.5f;
    s->x4C = 2.0f;
}

static inline void InitSub160(Sub160 *s)
{
    s->x30 = 2.0943952f;
}

void func_0011D488(Obj *o, void *owner, Model *model, Model *model2, int variant, void *part)
{
    Vec scale;
    void *p; /* one temp for both x1A0 and x14: needed for the original schedule */
    Part *xfc;

    o->x30 = owner;
    o->x10 = model;
    o->x14 = model2;
    scale.x = 1.0f;
    scale.y = 1.0f;
    scale.z = 1.0f;
    xfc = &o->xFC;
    o->x262 = func_00222BD8(1, 30);
    o->xFC.x0 = D_003041B0;
    xfc->x4 = part ? part : func_00163D40(D_003041B0, 0);
    xfc->xA = -1;
    xfc->x8 = -1;
    InitSub110(&o->x110);
    InitSub160(&o->x160);
    o->x250 = D_0029C164[0];
    o->x26A = 3600;
    o->x38 = 0;
    p = o->x1A0;
    if (p) {
        func_0022DDD8(p);
        o->x1A0 = 0;
    }
    func_001CFA00(o->x1A8, &o->snd[3]);
    func_0011E568(o, 0);

    if (o->x10) {
        o->x10->flags |= (SceneNodeFlags)1 << 57;
        o->x10->flags |= (SceneNodeFlags)1 << 56;
        if (o->x10->xB4 == 0) {
            o->x268 = variant;
            o->x10->xB4 = RegisterObjectShortRetail((void *)(o), (unsigned int)(((Hdr *)o->x10->resource)));
            ((Hdr *)o->x10->resource)->f.index = o->x10->xB4;
        }
        switch (((Hdr *)o->x10->resource)->word >> 18) {
        /* each case takes its own s160 (assigned once), so gcc can tell its stores
           apart from the stack vector `scale`; one shared pointer changes the schedule */
        case 0x4BA: {
            float t, a;
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[0], 0x4BA, variant, 1);
                func_00203590(&o->snd[1], 0x4BA, variant, 2);
                func_00203590(&o->snd[3], 0x4BA, variant, 3);
                func_00203590(&o->snd[10], 0x4BA, variant, 4);
                func_00203590(&o->snd[11], 0x4BA, variant, 5);
                o->x224 = 1.2f;
                o->x228 = 1.4f;
                o->x4 = 4;
                o->x200 = 0.024444444f;
                o->x204 = 0.12222222f;
                o->x208 = 0.65999997f;
                t = 0.90444446f;
                o->x20C = t;
                o->x254 = 5.0f;
                o->x258 = 3.0f;
                a = t * 60.0f;
                scale.x = 1.5f;
                scale.y = 1.5f;
                s160->x34 = a + a;
                scale.z = 1.5f;
                s160->x3C = a;
                s160->x38 = a + a;
            }
            break;
        }
        case 0x4BF:
        case 0x4C1:
        case 0x4C3: {
            float a;
            float k = 0.024444444f;
            {
                Sub160 *s160 = &o->x160;

                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 1);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                o->x200 = k;
                o->x224 = 1.1f;
                o->x228 = 1.3f;
                o->x4 = 1;
                o->x204 = 0.12222222f;
                o->x208 = func_00222B68(15.0f, 18.0f) * k;
                o->x20C = func_00222B68(22.0f, 25.0f) * k;
                o->x254 = 4.0f;
                o->x258 = 2.0f;
                a = o->x20C * 60.0f;
                s160->x34 = a;
                s160->x3C = a;
                s160->x38 = a;
            }
            break;
        }
        case 0x4C9: {
            float t, a;
            float k, s;
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[2], 0x4C9, variant, 1);
                k = 0.024444444f;
                s = 0.8f;
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                o->x224 = s;
                o->x200 = k;
                o->x258 = 1.0f;
                scale.x = 1.25f;
                scale.y = 1.25f;
                scale.z = 1.25f;
                o->x228 = 1.2f;
                o->x254 = 3.0f;
                o->x4 = 5;
                o->x204 = 0.12222222f;
                o->x208 = func_00222B68(15.0f, 18.0f) * k;
                t = func_00222B68(24.0f, 27.0f) * k;
                o->x20C = t;
                a = t * 60.0f;
                s160->x34 = a + a;
                s160->x3C = a;
                s160->x38 = a + a;
                o->x1A4 = s;
            }
            break;
        }
        case 0x4C6:
            if (func_00222B08(0, 1) || D_004B2840[0] == 0x13) {
                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 1);
            } else {
                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
            }
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[1], ((Hdr *)o->x10->resource)->word >> 18, variant, 3);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 4);
                func_00203590(&o->snd[4], ((Hdr *)o->x10->resource)->word >> 18, variant, 5);
                func_00203590(&o->snd[5], ((Hdr *)o->x10->resource)->word >> 18, variant, 6);
                func_00203590(&o->snd[6], ((Hdr *)o->x10->resource)->word >> 18, variant, 7);
                o->x254 = 4.0f;
                o->x258 = 2.0f;
                o->x230 = 0.6f;
                o->x234 = 0.6f;
                o->x224 = 0.8f;
                o->x228 = 1.2f;
                o->x4 = 3;
                o->x200 = 0.024444444f;
                o->x204 = 0.12222222f;
                o->x208 = 0.58666664f;
                o->x20C = 0.85555553f;
                scale.x = 1.5f;
                scale.y = 1.5f;
                s160->x34 = 51.333332f;
                s160->x3C = 51.333332f;
                s160->x38 = 51.333332f;
                scale.z = 1.5f;
            }
            break;
        case 0x4B8: {
            float t, a;
            float k;
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[2], 0x4B8, variant, 1);
                k = 0.024444444f;
                func_00203590(&o->snd[1], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 3);
                func_00203590(&o->snd[7], ((Hdr *)o->x10->resource)->word >> 18, variant, 7);
                func_00203590(&o->snd[8], ((Hdr *)o->x10->resource)->word >> 18, variant, 8);
                func_00203590(&o->snd[9], ((Hdr *)o->x10->resource)->word >> 18, variant, 9);
                o->x204 = k;
                o->x224 = 1.2f;
                o->x228 = 1.4f;
                o->x254 = 3.0f;
                o->x4 = 2;
                o->x200 = 0.0048888889f;
                o->x258 = 1.0f;
                o->x208 = func_00222B68(12.0f, 15.0f) * k;
                t = func_00222B68(18.0f, 21.0f) * k;
                o->x20C = t;
                a = t * 30.0f;
                s160->x34 = a * 4.0f;
                s160->x3C = a + a;
                s160->x38 = a + a;
                o->x1A4 = 0.8f;
            }
            break;
        }
        case 0x4D3:
        case 0x5D1:
        case 0x5D3: {
            float t, a, s;
            float k;
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 1);
                func_00203590(&o->snd[1], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 3);
                func_00203590(&o->snd[7], ((Hdr *)o->x10->resource)->word >> 18, variant, 4);
                func_00203590(&o->snd[8], ((Hdr *)o->x10->resource)->word >> 18, variant, 6);
                func_00203590(&o->snd[9], ((Hdr *)o->x10->resource)->word >> 18, variant, 5);
                o->x224 = 1.2f;
                o->x228 = 1.4f;
                o->x234 = 0.5f;
                o->x230 = 0.5f;
                o->x254 = 3.0f;
                o->x258 = 1.0f;
                o->x4 = 7;
                if (D_004B2840[0] == 0 || D_004B2840[0] == 0x18) {
                    scale.x = 0.6f;
                    scale.y = 0.6f;
                    scale.z = 0.6f;
                } else if (D_004B2840[0] == 0x14) {
                    scale.x = 0.75f;
                    scale.y = 0.75f;
                    scale.z = 0.75f;
                } else {
                    scale.x = 0.85f;
                    scale.y = 0.85f;
                    scale.z = 0.85f;
                }
                k = 0.024444444f;
                o->x200 = 0.0073333336f;
                o->x204 = 0.036666665f;
                o->x208 = func_00222B68(18.0f, 21.0f) * k;
                t = func_00222B68(35.0f, 40.0f) * k;
                o->x20C = t;
                a = t * 60.0f;
                s160->x34 = a * 4.0f;
                s160->x3C = a + a;
                s160->x38 = a + a;
                o->x1A4 = 0.6f;
            }
            break;
        }
        case 0x4D5:
            {
                Sub160 *s160 = &o->x160;
                func_00203590(&o->snd[2], 0x4D5, variant, 1);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                o->x224 = 1.0f;
                o->x228 = 1.2f;
                o->x4 = 9;
                o->x254 = 5.0f;
                o->x200 = 0.024444444f;
                o->x204 = 0.12222222f;
                o->x208 = 0.36666667f;
                o->x20C = 0.44f;
                o->x258 = 2.0f;
                scale.x = 2.0f;
                scale.y = 2.0f;
                s160->x34 = 79.2f;
                s160->x3C = 79.2f;
                s160->x38 = 79.2f;
                scale.z = 2.0f;
            }
            break;
        case 0x4CB:
        case 0x4E3:
        case 0x4E5: {
            float a;
            float k = 0.48888889f;
            {
                Sub160 *s160 = &o->x160;

                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 1);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                func_00203590(&o->snd[7], ((Hdr *)o->x10->resource)->word >> 18, variant, 4);
                func_00203590(&o->snd[8], ((Hdr *)o->x10->resource)->word >> 18, variant, 6);
                func_00203590(&o->snd[9], ((Hdr *)o->x10->resource)->word >> 18, variant, 5);
                o->x208 = 0.24444444f;
                o->x20C = k;
                o->x254 = 1.5f;
                o->x258 = 0.5f;
                if (((Hdr *)o->x10->resource)->f.id == 0x4CB) {
                    if (D_004B25A0.x2A0 == 1 || D_004B25A0.x2A0 == 0xD) {
                        o->x224 = 1.0f;
                        o->x228 = 1.4f;
                        func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 3);
                        o->x208 = k;
                        o->x20C = 0.73333335f;
                    } else {
                        o->x224 = 1.2f;
                        o->x228 = 1.4f;
                    }
                } else {
                    o->x224 = 2.0f;
                    o->x228 = 2.4f;
                }
                o->x4 = 10;
                scale.x = 1.0f;
                scale.y = 1.0f;
                scale.z = 1.0f;
                a = o->x20C * 60.0f;
                s160->x34 = a * 4.0f;
                s160->x3C = a + a;
                s160->x38 = a + a;
                if (D_004B25A0.x2A0 == 0x14) {
                    scale.x = 2.0f;
                    scale.y = 2.0f;
                    scale.z = 2.0f;
                    o->x208 = 0.36666667f;
                    o->x20C = 0.6111111f;
                }
                o->x1A4 = 0.5f;
            }
            break;
        }
        case 0x5CD:
        case 0x5CF: {
            float t, a;
            float s = 0.5f;
            float k = 0.024444444f;
            {
                Sub160 *s160 = &o->x160;

                func_00203590(&o->snd[2], ((Hdr *)o->x10->resource)->word >> 18, variant, 1);
                func_00203590(&o->snd[1], ((Hdr *)o->x10->resource)->word >> 18, variant, 2);
                func_00203590(&o->snd[3], ((Hdr *)o->x10->resource)->word >> 18, variant, 3);
                func_00203590(&o->snd[7], ((Hdr *)o->x10->resource)->word >> 18, variant, 4);
                func_00203590(&o->snd[8], ((Hdr *)o->x10->resource)->word >> 18, variant, 6);
                func_00203590(&o->snd[9], ((Hdr *)o->x10->resource)->word >> 18, variant, 5);
                o->x224 = 2.0f;
                o->x228 = 2.4f;
                o->x234 = s;
                o->x230 = s;
                o->x4 = 11;
                o->x254 = 1.5f;
                o->x200 = 0.0073333336f;
                o->x204 = 0.036666665f;
                o->x258 = s;
                o->x208 = func_00222B68(18.0f, 21.0f) * k;
                t = func_00222B68(35.0f, 40.0f) * k;
                o->x20C = t;
                a = t * 60.0f;
                s160->x34 = a;
                s160->x3C = a;
                s160->x38 = a;
                o->x1A4 = s;
                if (D_004B2840[0] == 0x14) {
                    scale.x = 0.8f;
                    scale.y = 0.8f;
                    scale.z = 0.8f;
                }
            }
            break;
        }
        }
        COPY_VECTOR(o->x10->x30, &scale);
    }
    p = o->x14;
    if (p) {
        ((Model *)p)->flags |= (SceneNodeFlags)1 << 56;
        COPY_VECTOR(o->x14->x30, &scale);
    }
    func_0011E860(o);
}
