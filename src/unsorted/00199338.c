/* UNIT 0x00199338 0x0019A0D8 .sdata=0x29D100:0x29D110 */
#include "soft_double.h"
#include "include_asm.h"

extern char D_002AF2C0[];
typedef float Vec4[4] __attribute__((aligned(16)));

typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x10];
    Vec pos;
    char pad20[0x20];
    Vec vel;
} Body;

typedef struct Owner {
    char pad0[0x1184];
    int x1184;
    char pad1188[0x6C];
    int x11F4;
    int x11F8;
    char pad11FC[0x50];
    int x124C;
    char pad1250[0x8];
    int x1258;
    int x125C;
    char pad1260[0x174];
    unsigned char x13D4;
    char pad13D5[0x2];
    unsigned char x13D7;
    unsigned char x13D8;
    char pad13D9[0x4FB7];
    int x6390;
    char pad6394[0x1598];
    Body *x792C;
    char pad7930[0x48];
    int x7978;
    char pad797C[0xEC];
    unsigned char x7A68;
    char pad7A69[0xF];
    float x7A78;
} Owner;

typedef struct Obj {
    char pad0[0x28];
    Owner *x28;
    char pad2C[4];
    Vec4 x30[4];
    Vec4 x70[9];
    float x100;
    unsigned char x104;
    unsigned char x105;
    unsigned char x106;
    unsigned char x107;
} Obj;

void func_001999A8(Obj *obj, void *other, float *delta, float distSq);
extern float fabsf(float);

int D_0029D100 = 0;
unsigned char D_0029D104 = 1;
int D_0029D108 = 0;
unsigned char D_0029D10C = 1;
extern const float D_0029D110[];
extern struct { char pad0[0x11A70]; unsigned char x11A70[1]; } D_004225A0;
/* bike position (x, y, z) and heading */
typedef struct Pos {
    float x, y, z, w;
} Pos;

extern Pos D_0043C820;

struct ResetObj;
extern void func_00199A48(struct ResetObj *);
extern int func_002205A0(int);
extern void func_00220198(int, int);
extern void func_00221888(Vec *, Vec *, Vec *);

void func_00199338(Obj *obj, Owner *owner)
{
    int i;
    int handle;

    obj->x28 = owner;
    for (i = 0; i < 4; i++) {
        __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(obj->x30[i]) : "memory");
    }
    for (i = 0; i < 9; i++) {
        __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(obj->x70[i]) : "memory");
    }
    obj->x100 = D_0029D110[0];
    obj->x104 = 0;
    obj->x105 = 0;
    obj->x106 = 0;
    obj->x107 = 0;
    func_00199A48((struct ResetObj *)obj);
    if (obj->x28) {
        handle = func_002205A0(obj->x28->x7978);
        func_00220198(handle, D_004225A0.x11A70[obj->x28->x7978] != 0);
    }
    D_0029D104 = 1;
    D_0029D100 = 0;
    for (i = 0; i < 4; i++) {
        ((int *)&D_0043C820)[i] = 0;
    }
    D_0029D108 = 0;
    D_0029D10C = 1;
}

typedef struct Input {
    char pad0[0x1B0];
    int x1B0;
    int x1B4;
    char pad1B8[0xC0];
    int x278;
    int x27C;
    char pad280[0x177];
    unsigned char x3F7;
    unsigned char x3F8;
} Input;

typedef struct Grid {
    char pad0[0x1004C];
    int x1004C;
} Grid;

extern Owner *D_0043EEC0[];
extern int D_003F5400[];
extern int D_004B1F40[];
extern Grid D_0029EA20;

extern int func_00220218(int);
extern int func_00238F48(void);
extern void func_001B45F8(int *, int, int);
extern Vec *func_001177F8(Grid *, int);
extern void func_0017DC78(Input *);
extern void func_0017DCA0(Input *);
extern void func_0017DD70(Input *, int, int);
extern void func_001997C8();
extern void func_001998C0();

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
"
        "lqc2 $vf12, 0x0(%2)
"
        "vsub.xyz $vf11, $vf11, $vf12
"
        "sqc2 $vf11, %0
"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile (
        "mula.s %1, %2
"
        "madda.s %3, %4
"
        "madd.s %0, %5, %6
"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline void SetActive(Owner *rider, int value)
{
    if (rider->x13D4) rider->x124C = value;
    else { rider->x1184 = value; rider->x124C = 0; }
}

static inline void SetInputForward(Input *in, int value)
{
    if (in->x3F7) in->x278 = value;
    else { in->x1B0 = value; in->x278 = 0; }
}
static inline void SetInputBack(Input *in, int value)
{
    if (in->x3F8) in->x27C = value;
    else { in->x1B4 = value; in->x27C = 0; }
}
static inline int GetForward(Owner *rider)
{
    if (rider->x13D7) return rider->x1258;
    return rider->x11F4;
}
static inline int GetBack(Owner *rider)
{
    if (rider->x13D8) return rider->x125C;
    return rider->x11F8;
}

void func_00199458(Obj *obj)
{
    register Input *in;
    Owner *rider;
    Owner *o28;
    Vec delta;
    Vec *pos;
    float dist, inv, dot, thresh;
    int flag, sel;

    if (obj->x28) {
        if (!func_00220218(func_002205A0(obj->x28->x7978))) {
            if (func_00238F48() >= 5) {
                func_001B45F8(D_004B1F40, 4, 0);
                D_003F5400[0] = obj->x28->x7978;
            }
        } else {
            if (obj->x28->x7A68 && (rider = obj->x28, rider == D_0043EEC0[0])) {
                in = (Input *)((char *)rider + 0xFE0);
                func_0017DC78(in);
                func_0017DCA0(in);
                if (obj->x106) {
                    SetActive(rider, 1);
                } else {
                    pos = func_001177F8(&D_0029EA20, D_0029EA20.x1004C - 1);
                    if (pos) {
                        VecSub(&delta, pos, &obj->x28->x792C->pos);
                        __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0" : "=&f"(dist) : "f"(delta.x), "f"(delta.y), "f"(delta.z));
                        if (dist < 20.0f || dist > 300.0f) {
                            obj->x106 = 1;
                            SetActive(rider, flag = 1);
                        } else {
                            flag = 0;
                            dist = VecDot(&delta, &delta);
                            __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(dist));
                            delta.x *= inv;
                            delta.y *= inv;
                            delta.z *= inv;
                            dot = VecDot(&obj->x28->x792C->vel, &delta);
                            if (dot > 0.0f) {
                                sel = GetForward(rider);
                                thresh = 0.2f;
                                if (sel) thresh = 0.05f;
                                if (thresh < dot) flag = 1;
                                if (flag) {
                                    SetInputForward(in, 1);
                                    obj->x107 = 0;
                                }
                            } else {
                                sel = GetBack(rider);
                                thresh = -0.2f;
                                if (sel) thresh = -0.05f;
                                if (dot < thresh) flag = 1;
                                if (flag) {
                                    SetInputBack(in, 1);
                                    obj->x107 = 1;
                                }
                            }
                        }
                    }
                }
            } else {
                Owner *q;
                func_0017DD70((Input *)((char *)obj->x28 + 0xFE0), obj->x28->x7978, obj->x28->x6390);
                q = obj->x28;
                if (q->x7A68 && q->x7A78 == 0.0f) {
                    SetActive(q, 1);
                }
            }
            obj->x104 = 0;
            obj->x105 = 0;
            obj->x100 = 1.1e15f;
            func_001997C8(obj);
            func_001998C0(obj);
        }
    }
}


typedef struct NearBody {
    char pad0[0x10];
    Vec x10;
} NearBody;

typedef struct NearRider {
    char pad0[0x792C];
    NearBody *x792C;
    char pad7930[0x48];
    int x7978;
} NearRider;

typedef struct NearGame {
    char pad0[0x50130];
    NearRider *riders[1];
} NearGame;

typedef struct NearGlobal {
    char pad0[0x288];
    int x288;
} NearGlobal;

typedef struct NearObj {
    char pad0[0x28];
    NearRider *x28;
    char pad2C[0x4];
    Vec x30[13];
    float x100;
} NearObj;

extern NearGlobal D_004B25A0;
#define NEAR_GAME (*(NearGame *)&D_002AF2C0)


/* Tracks the nearest other rider: x30[i] = offset to rider i, x100 = smallest distance squared. */
void func_001997C8(NearObj *obj)
{
    NearRider *other;
    float dist;
    int i;

    for (i = 0; i < D_004B25A0.x288; i++) {
        if (i != obj->x28->x7978) {
            other = NEAR_GAME.riders[i];
            VecSub(&obj->x30[i], &other->x792C->x10, &obj->x28->x792C->x10);
            dist = VecDot(&obj->x30[i], &obj->x30[i]);
            if (dist < obj->x100) {
                obj->x100 = dist;
                func_001999A8(obj, other, &obj->x30[i], dist);
            }
        }
    }
}

typedef struct Body2 {
    char pad0[0x10];
    float pos[4];
} Body2;

typedef struct Rider2 {
    char pad0[0x792C];
    Body2 *body;
} Rider2;

typedef struct Obj2 {
    char pad0[0x28];
    Rider2 *rider;
    char pad2C[0x70 - 0x2C];
    float delta[9][4];
    float minDistSq;    /* 0x100 */
} Obj2;

struct Global2 {
    char pad[0x50140];
    Rider2 *riders[26];
    int count;
    int n;
};


extern int D_004B2828[];



void func_001998C0(Obj2 *obj)
{
    struct Global2 *global;
    Rider2 **riders;
    float *delta;
    int count;
    int n;
    float distSq;
    Rider2 *other;

    global = (struct Global2 *)&D_002AF2C0;
    n = global->count - D_004B2828[0];
    if (n > 0) {
        count = n;
        delta = obj->delta[0];
        riders = global->riders;
        do {
            other = *riders;
            __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(delta), "r"(other->body->pos), "r"(obj->rider->body->pos) : "memory");
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(distSq) : "f"(delta[0]), "f"(delta[1]), "f"(delta[2]));
            if (distSq < obj->minDistSq) {
                obj->minDistSq = distSq;
                func_001999A8(obj, other, delta, distSq);
            }
            delta += 4;
            count--;
            riders++;
        } while (count != 0);
    }
}

/* other and distSq are unused; kept so the callers' argument list still matches */
void func_001999A8(Obj *obj, void *other, float *delta, float distSq)
{
    float local[4] __attribute__((aligned(16)));
    float absY;
    float y;

    func_00221888(&obj->x28->x792C->vel, delta, (Vec *)local);
    if (obj->x28->x6390 != 4) {
        y = local[1];
        local[1] = y;
        absY = fabsf(y);
        __asm__ volatile("L9a8a:" : : "f"(y));
        if (!(absY < 20.0f)) return;
        __asm__ volatile("L9a8b:" : : "f"(absY));
    }
    if (local[0] > 0.0f) {
        obj->x104 = 0;
        obj->x105 = 1;
    } else {
        obj->x105 = 0;
        obj->x104 = 1;
    }
}

typedef struct Sub {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
} Sub;

typedef struct ResetObj {
    Sub cur;
    Sub prev;
} ResetObj;

void func_00199A48(ResetObj *obj)
{
    obj->cur.x0 = 0;
    obj->cur.xC = 8;
    obj->cur.x4 = 0;
    obj->cur.x8 = 0;
    obj->cur.x10 = 0;
    obj->prev = obj->cur;
}

typedef struct PadObj {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
} PadObj;

/* The div/mod survive in retail (constants only reach them after inlining). */
static __inline__ void Obj_Set(PadObj *obj, int value, int div)
{
    int q, r;

    obj->x8 = value;
    q = value / div;
    r = value % div;
    obj->xC = 1;
    obj->x1C = value;
    obj->x20 = 8;
    obj->x10 = 0;
    obj->x18 = r;
    obj->x4 = q;
}

void func_00199A90(PadObj *obj)
{
    Obj_Set(obj, 255, 10);
}

typedef struct RumbleRider { char pad0[0x4566]; short x4566; char pad4568[0x7978-0x4568]; int x7978,x797c; } RumbleRider;
typedef struct State { int x0,x4,x8,xc,x10,x14,x18,x1c,x20,x24; RumbleRider *x28; } State;
typedef struct Entry { char pad0[16]; int x10; unsigned char x14; char pad15[11]; } Entry;
extern Entry D_00768098[];
extern int func_00220180(int);
extern int func_00162D80(int);
extern void func_00220018(int,unsigned char,unsigned char,int);
void func_00199AD8(State *s,float value) {
 int amount,index,handle; Entry *e;
 if(s->x28 && func_00220180(func_002205A0(s->x28->x7978))) {
  amount=(int)(value*0.03f);
  if(s->x28->x4566>=11) amount*=5;
  amount=amount>=256?255:amount;
  index=func_00162D80(s->x28->x797c);
  if(index>=32) e=D_00768098; else e=&D_00768098[index];
  s->x1c-=s->x20;
  if(s->x1c<amount) s->x1c=amount; else if(s->x1c<0) s->x1c=0;
  if(e->x10>s->x1c) s->x1c=e->x10;
  if(s->x1c) { int n=s->x1c/10; s->x18=n>0?n:1; } else s->x18=0;
  s->x24=0;
  handle=func_002205A0(s->x28->x7978);
  if(s->x1c) func_00220018(handle,0,(unsigned char)s->x1c,s->x18);
  if(--s->x4>0) { s->x8-=s->xc; if(s->x8<0) s->x8=0; }
  if(s->x8<e->x14) { s->x8=e->x14; s->x4=1; }
  if(s->x8) func_00220018(handle,(unsigned char)s->x8,0,s->x4);
 }
}

extern char D_0028DAA0[];
extern char D_0028DAB0[];
extern char D_0028DAC0[];
extern char D_0028DAD0[];
extern char D_0028DAE0[];

extern int func_00220618(int button, int pad);
extern int func_00220AE0(int axis, int pad);
extern void func_00208888(float *v, float angle);
extern void func_0020F518(int ctx, int r, int g, int b, int a);
extern void func_0020FB40(int ctx, int a, int b, int c, int d);
extern void func_0020F9A8(int ctx, int font, int x, int y, char *text);

extern int func_00254D68(char *buf, const char *fmt, ...);

void func_00199CA0(void)
{
    char buf[64];
    int delta;

    if (D_0029D104) {
        if (func_00220618(0x20, 0)) {
            delta = 1;
        } else if (func_00220618(0x80, 0)) {
            delta = -1;
        } else {
            int value = func_00220AE0(2, 0);
            if (value > 0x78) {
                delta = 10;
            } else if (value < -0x78) {
                delta = -10;
            } else {
                delta = 0;
            }
        }
        if (D_0029D108 > 0 && D_0029D108 < 30) {
            D_0029D10C = 0;
        } else {
            D_0029D10C = 1;
        }
        if (delta) {
            D_0029D108++;
            if (D_0029D10C) {
                (&D_0043C820.x)[D_0029D100] += delta;
            }
            if (D_0029D100 == 3) {
                func_00208888(&(&D_0043C820.x)[D_0029D100], 180.0f);
            }
        } else {
            D_0029D108 = 0;
            D_0029D10C = 1;
            if (func_00220618(0x10, 0)) {
                int index;

                D_0029D104 = 0;
                index = D_0029D100 - 1;
                if (index < 0) {
                    index = 0;
                }
                D_0029D100 = index;
            } else if (func_00220618(0x40, 0)) {
                int index, max = 3;

                D_0029D104 = 0;
                index = D_0029D100 + 1;
                D_0029D100 = (index > max) ? max : index;
            }
        }
    } else if (!func_00220618(1, 0) && !func_00220618(0x20, 0) && !func_00220618(0x80, 0)
               && !func_00220618(0x10, 0) && !func_00220618(0x40, 0)) {
        D_0029D104 = 1;
    }

    func_0020F518(0, 0xC0, 0xC0, 0xC0, 0x96);
    func_0020FB40(0, 0x13, 0xC, -9, -2);
    func_00254D68(buf, D_0028DAA0);
    func_0020F9A8(0, 0, 0x1E, 0x3C, buf);

    if (D_0029D100 == 0) {
        func_0020F518(0, 0xC0, 0xC0, 0, 0xB4);
    } else {
        func_0020F518(0, 0xC0, 0xC0, 0xC0, 0x96);
    }
    func_00254D68(buf, D_0028DAB0, func_001FD300(D_0043C820.x));
    func_0020F9A8(0, 0, 0x1E, 0x46, buf);

    if (D_0029D100 == 1) {
        func_0020F518(0, 0xC0, 0xC0, 0, 0xB4);
    } else {
        func_0020F518(0, 0xC0, 0xC0, 0xC0, 0x96);
    }
    func_00254D68(buf, D_0028DAC0, func_001FD300(D_0043C820.y));
    func_0020F9A8(0, 0, 0x1E, 0x50, buf);

    if (D_0029D100 == 2) {
        func_0020F518(0, 0xC0, 0xC0, 0, 0xB4);
    } else {
        func_0020F518(0, 0xC0, 0xC0, 0xC0, 0x96);
    }
    func_00254D68(buf, D_0028DAD0, func_001FD300(D_0043C820.z));
    func_0020F9A8(0, 0, 0x1E, 0x5A, buf);

    if (D_0029D100 == 3) {
        func_0020F518(0, 0xC0, 0xC0, 0, 0xB4);
    } else {
        func_0020F518(0, 0xC0, 0xC0, 0xC0, 0x96);
    }
    func_00254D68(buf, D_0028DAE0, func_001FD300(D_0043C820.w));
    func_0020F9A8(0, 0, 0x1E, 0x64, buf);
}
