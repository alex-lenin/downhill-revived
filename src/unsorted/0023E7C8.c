/* UNIT 0x0023E7C8 0x00240080 .sdata=0x29DCB8:0x29DCC8 */
#include "particle_spray.h"
#include "particle.h"
#include "include_asm.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef ParticleSprayVector Vec;

typedef ParticleSprayEmitter Obj;

typedef struct Entry { char pad0[6]; unsigned short x6; char pad8[0x38]; } Entry;

float D_0029DCB8 = 0.0625f;
int D_0029DCBC = 40;
float D_0029DCC0 = -0.017f;
unsigned int D_0029DCC4 = 0x4F29DB62;

extern int D_002694D0[];
extern int D_0027F6C0[];
extern Vec D_0027F680, D_0027F690, D_0027F6A0, D_0027F6B0;
extern Entry D_0027BFC0[];
extern void func_00223628(Vec *, Vec *);
extern Obj *func_0022DD88(int);

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VSCALE(dst,a,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(f) : "$2")
#define VNEG(dst,a) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tvsub.xyz $vf11,$vf0,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a))

static inline void VAdd(Vec *dst, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline void VSub(Vec *dst, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b));
}
static inline void VMadd(Vec *dst, Vec *a, Vec *b, float f) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b),"f"(f) : "$2");
}

/* LCG: a float in [1,2) built from the top mantissa bits of the seed */
static inline float Rand01(void) {
    union { unsigned int i; float f; } u;
    D_0029DCC4 = D_0029DCC4 * 0x41C64E6D + 12345;
    u.i = ((D_0029DCC4 >> 9) & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}
#define VSCALE4(dst,a,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyzw $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(f) : "$2")
/* dst.xy = m0 * v.x + m1 * v.y (the loads are separate statements: retail
   materialises each row address between them) */
#define ROTATE_XY(dst,src,m0,m1) { \
    Vec *v_ = (src); \
    __asm__ volatile ("lqc2 $vf11,0(%0)" : : "r"(v_)); \
    __asm__ volatile ("lqc2 $vf12,0(%0)" : : "r"(&(m0))); \
    __asm__ volatile ("lqc2 $vf13,0(%0)" : : "r"(&(m1))); \
    __asm__ volatile ("vmulax.xy ACC,$vf12,$vf11x\n\tvmaddy.xy $vf11,$vf13,$vf11y\n\tsqc2 $vf11,%0" : "=m"(*(dst))); \
}
/* grow the bounding box kept in $vf30 (min) / $vf31 (max) */
static inline void GrowBounds(Vec *a) {
    __asm__ volatile ("lqc2 $vf11,0(%0)\n\tvmini.xyzw $vf30,$vf30,$vf11\n\tvmax.xyzw $vf31,$vf31,$vf11" : : "r"(a));
}
static inline void NormalizeXYZ(Vec *v) {
    float x = v->f[0], y = v->f[1], z = v->f[2], len, inv;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len) : "f"(x),"f"(y),"f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
    v->f[0] = x * inv; v->f[1] = y * inv; v->f[2] = z * inv;
}

/* Emits one particle into each of the three trail sections (rings at
   x285C/x285D/x285E) and refreshes the bounding box x2610/x2620. */
void func_0023E7C8(Obj *obj) {
    Vec dir, side, tmp;
    Vec *a, *pos, *vel;
    int *color;
    short *duration;
    int i, count, packed, n;
    float root, w, distance, life, invlife, cap;
    union { float f; int i; } bits;

    VSCALE4(dir, &obj->x2700, ((unsigned char *)&obj->x2828)[3] * (1.0f / 255.0f));
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(root) : "f"(dir.f[3]));
    VSCALE4(dir, &dir, root / dir.f[3]);
    w = dir.f[3];
    n = (int)(w * obj->x276E * obj->x272C) & 0xFC;
    count = D_0029DCBC;
    if (count >= n) count = n;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(cap) : "f"((float)obj->x2724),"f"((float)obj->x276E));
    distance = -w * obj->x2744 * cap * obj->x274C;
    tmp.q = 0;
    tmp.f[0] = dir.f[1];
    tmp.f[1] = -dir.f[0];
    side = tmp;
    invlife = 1.0f / (float)obj->x27DA;
    NormalizeXYZ(&side);
    life = (float)obj->x27DA * w;

    i = obj->x285C;
    {
        short *d0 = (short *)(i * 2 + (int)obj->x2838);
        a = &obj->x2830[i * 2]; pos = &obj->x2830[i * 2 + 1];
        vel = &obj->x2834[i]; color = &obj->x283C[i];
        *d0 = (int)(life * 0.75f);
    }
    a->f[0] = side.f[0]; a->f[1] = side.f[1];
    a->f[2] = Rand01(); a->f[3] = Rand01();
    VMadd(pos, &obj->x2690, &obj->x26A0, distance * 0.75f);
    VMadd(pos, pos, &dir, -6.0f / dir.f[3]);
    bits.f = 0.0f;
    packed = bits.i;
    packed = (packed & 0xFFFFFF00) | count;
    pos->i[3] = packed;
    VSCALE(*vel, &dir, obj->x2748);
    VMadd(vel, vel, &obj->x26A0, dir.f[3] * obj->x2744 * 0.75f);
    vel->f[3] = invlife;
    *color = obj->x2828;
    if (obj->x2850 == 0) {
        __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%0)" : : "r"(pos));
    } else {
        __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%1)" : : "r"(&obj->x2610),"r"(&obj->x2620));
        GrowBounds(pos);
    }

    i = obj->x285D;
    duration = &obj->x2838[i + 42];
    a = &obj->x2830[i * 2 + 85]; pos = &obj->x2830[i * 2 + 86];
    vel = &obj->x2834[i + 42]; color = &obj->x283C[i + 48];
    *duration = (int)life;
    ROTATE_XY(a, &side, D_0027F680, D_0027F690);
    a->f[2] = Rand01(); a->f[3] = Rand01();
    VMadd(pos, &obj->x2690, &obj->x26A0, distance);
    VMadd(pos, pos, &dir, -2.0f / dir.f[3]);
    pos->i[3] = packed;
    VSCALE(*vel, &dir, obj->x2748);
    VMadd(vel, vel, &obj->x26A0, dir.f[3] * obj->x2744);
    VMadd(vel, vel, &side, -obj->x2740);
    vel->f[3] = invlife;
    *color = obj->x2828;
    GrowBounds(pos);

    i = obj->x285E;
    duration = &obj->x2838[i + 84];
    a = &obj->x2830[i * 2 + 170]; pos = &obj->x2830[i * 2 + 171];
    vel = &obj->x2834[i + 84]; color = &obj->x283C[i + 96];
    *duration = (int)life;
    ROTATE_XY(a, &side, D_0027F6A0, D_0027F6B0);
    a->f[2] = Rand01(); a->f[3] = Rand01();
    VMadd(pos, &obj->x2690, &obj->x26A0, distance);
    VMadd(pos, pos, &dir, -2.0f / dir.f[3]);
    pos->i[3] = packed;
    VSCALE(*vel, &dir, obj->x2748);
    VMadd(vel, vel, &obj->x26A0, dir.f[3] * obj->x2744);
    VMadd(vel, vel, &side, obj->x2740);
    vel->f[3] = invlife;
    *color = obj->x2828;
    GrowBounds(pos);
    func_00223628(&obj->x2610, &obj->x2620);
}

void func_0023EE58(Obj **handle,int id,Vec *position,Vec *direction,Vec *velocity,int color) {
 Vec pos,dir,vel;
 Vec *value;
 float length;
 Entry *entry;
 pos=*position;
 dir=*direction;
 vel=*velocity;
 value=&vel;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(value->f[0]),"f"(value->f[1]),"f"(value->f[2]));
 value->f[3]=length;
 if(D_0029DCB8<length) {
  if(!*handle) {
   *handle=func_0022DD88(func_0022CDC0((Vector4 *)(&pos), id, (Matrix4 *)(0), (int)(0), 1.0f, (Vector4 *)(0)));
   if(!*handle) return;
   (*handle)->x2848=handle;
   (*handle)->x276E=(*handle)->x2724 >> 1;
   if(!*handle) return;
  }
  if((*handle)->x276C==id) {
   entry=(Entry *)((*handle)->x276C << 6);
   entry=(Entry *)((unsigned int)entry+(unsigned int)D_0027BFC0);
   (*handle)->x2726=entry->x6;
   if((*handle)->x276E >= ((*handle)->x2724 >> 1)) {
    (*handle)->x2700=vel;
    (*handle)->x2690=pos;
    (*handle)->x26A0=dir;
    (*handle)->x2828=color;
    (*handle)->x285A=1;
   }
  }
 }
}

void func_0023EFB8(Obj *obj, int amount) {
    signed char *pc;
    signed char *p;
    signed char *end;
    signed char ch;
    int n;
    int m;
    int state;

    state = obj->x285A;
    if ((state ^ 1) == 0) {
        func_0023E7C8(obj);
        if (obj->x276E >= obj->x2724) {
            int *tbl = D_0027F6C0;
            int k, idx;
            signed char *dst;
            k = obj->x285C * 2 + 1;
            idx = k + tbl[0];
            dst = &obj->x2830[idx].b[12];
            *dst = 0;
            k = obj->x285D * 2 + 1;
            idx = k + tbl[1];
            dst = &obj->x2830[idx].b[12];
            *dst = 0;
            k = obj->x285E * 2 + 1;
            idx = k + tbl[2];
            dst = &obj->x2830[idx].b[12];
            *dst = 0;
            pc = &obj->x285C;
        } else {
            int *tbl;
            signed char *q;
            int k;
            pc = &obj->x285C;
            for (q = pc, tbl = D_0027F6C0, k = 2; k >= 0; q++, k--) {
                if (*q <= 0) {
                    int k2;
                    int idx;
                    signed char *dst;
                    k2 = *q * 2 + 1;
                    idx = k2 + *tbl;
                    dst = &obj->x2830[idx].b[12];
                    *dst = 0;
                }
                tbl++;
            }
        }

        p = pc;
        end = p + 3;
        do {
            ch = *p;
            if (ch < obj->x2854) {
                *p = ch + 1;
            }
            p++;
        } while ((int)p < (int)end);
        obj->x276E = 0;
    } else {
        if (obj->x2726 > 0) {
            obj->x2726 = (unsigned short)obj->x2726 - amount;
        }
    }

    n = obj->x285D;
    m = obj->x285E;
    if (n > m) m = n;
    obj->x285A = 0;
    obj->x2850 = m < obj->x285C ? obj->x285C : m;
}

#ifdef NONMATCHING
/* Non-matching: ring index lives in t9 instead of t3 (t3/t9/t0 rotated), D_002694D0/0xFFFFFF00 mask setup scheduled differently, one commutative mul.s operand order; best diff 44/325 words */
/* x27F0 / x27F4 (speed and life scales of the middle ring x285D and of the two side
   rings x285C/x285E) are not named in ParticleSprayEmitter (they sit in its pad27DC),
   so they are read through this overlay of the same object. */
typedef struct ParticleSprayScales {
    char pad0[0x27F0];
    float x27F0;
    float x27F4;
} ParticleSprayScales;
#define SPRAY_MID_SCALE(o)  (((ParticleSprayScales *)(o))->x27F0)
#define SPRAY_SIDE_SCALE(o) (((ParticleSprayScales *)(o))->x27F4)

/* Spawns one particle into each of the three rings (middle ring x285D first, then the
   side rings x285C and x285E) at *pos, and resets the bounding box to x2630. */
void func_0023F138(Obj *obj, Vec *pos) {
    Vec p, mid, q;
    Vec *pp = &p;
    int i, n, count, color, slot;
    short *d;
    float c, s, lo, range, inv, scale, rawScale, invlen, duration, one, minusEight, tmp;
    float nx, ny, nz, normSq;
    union { unsigned int i; float f; } bits;
    Vec *bD, *bC, *aC, *aE, *qDst, *dir, *side, *meta;

    p = *pos;
    duration = (float)obj->x27DA;
    obj->x285A = 1;
    one = 1.0f;
    minusEight = -8.0f;
    tmp = obj->x2730;
    c = one - (tmp + tmp);
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(s) : "f"(one - c * c));
    i = obj->x285D;
    count = (int)(obj->x273C * obj->x2740 / obj->x272C) & 0xFC;
    if (i) {
        count = D_0029DCBC < count ? D_0029DCBC : count;
    } else {
        count = 0;
    }
    inv = 1.0f / duration;
    bits.f = minusEight * inv;
    obj->x2630.i[3] = (bits.i & 0xFFFFFF00) | count;
    color = D_002694D0[obj->x281C];
    dir = &obj->x2640;
    COPY_VECTOR(q, dir);
    q.f[2] = 0.0f;
    nx = q.f[0];
    ny = q.f[1];
    nz = q.f[2];
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                      : "=f"(normSq) : "f"(nx), "f"(ny), "f"(nz));
    __asm__ volatile ("rsqrt.s %0,%1,%2"
                      : "=&f"(invlen) : "f"(1.0f), "f"(normSq));
    q.f[0] = nx * invlen;
    q.f[1] = ny * invlen;

    /* middle ring (x285D): sections at x2834[42..], x2830[84..], x283C[48..], x2838[42..] */
    bD = &obj->x2834[i + 42];
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    lo = obj->x2740;
    range = obj->x2744 - lo;
    scale = lo + Rand01() * range;
    VNEG(*pp, pp);

    side = &obj->x2660;
    rawScale = SPRAY_MID_SCALE(obj);
    rawScale = scale * rawScale;
    pp->f[3] = inv * SPRAY_MID_SCALE(obj);
    VMadd(bD, pp, side, rawScale);
    meta = &obj->x2630;
    slot = i * 2;
    VMadd(&obj->x2830[slot + 86], meta, bD, minusEight);
    qDst = &obj->x2830[slot + 85];
    *qDst = q;
    obj->x283C[i + 48] = color;
    d = (short *)(slot + (int)obj->x2838);
    d[42] = (short)(duration * SPRAY_MID_SCALE(obj));
    obj->x285D = i + 1;

    /* first side ring (x285C), sideways by +s */
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    i = obj->x285C;
    scale = (lo + Rand01() * range) * SPRAY_SIDE_SCALE(obj);
    bC = &obj->x2834[i];
    VMadd(&mid, pp, dir, s * scale);
    VMadd(bC, &mid, side, c * scale);
    slot = i * 2;
    VMadd(&obj->x2830[slot + 1], meta, bC, minusEight);
    aC = (Vec *)(slot * 16 + (int)obj->x2830);
    aC[0] = q;
    obj->x283C[i] = color;
    d = (short *)(slot + (int)obj->x2838);
    d[0] = (short)(duration * SPRAY_SIDE_SCALE(obj));
    obj->x285C = i + 1;

    /* second side ring (x285E), sideways by -s: sections at x2834[84..], x2830[170..], x283C[96..], x2838[84..] */
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    i = obj->x285E;
    scale = (lo + Rand01() * range) * SPRAY_SIDE_SCALE(obj);
    bC = &obj->x2834[i + 84];
    VMadd(&mid, pp, dir, -s * scale);
    VMadd(bC, &mid, side, c * scale);
    slot = i * 2;
    VMadd(&obj->x2830[slot + 171], meta, bC, minusEight);
    aE = (Vec *)(slot * 16 + (int)obj->x2830);
    aE[170] = q;
    obj->x283C[i + 96] = color;
    obj->x2838[i + 84] = (short)(duration * SPRAY_SIDE_SCALE(obj));
    n = (signed char)(i + 1);
    obj->x285E = i + 1;
    if (n < obj->x285D) n = obj->x285D;
    obj->x2850 = n < obj->x285C ? obj->x285C : n;
    __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%0)" : : "r"(&obj->x2630));
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0023F138);
#endif

void func_0023F650(Obj *obj, Vec *pos) {
    Vec p, side, up, fwd, down, mid, q;
    Vec *pp = &p;
    float c, s, lo, range;
    float half, sin60;
    int i, n, count, color;
    Vec *a, *a1, *b;
    int *colors;

    i = obj->x285C;
    p = *pos;
    a = &obj->x2830[i * 2];
    a1 = &obj->x2830[i * 2 + 1];
    b = &obj->x2834[i];
    colors = &obj->x283C[i];
    obj->x285A = 1;
    c = 1.0f - (obj->x2730 + obj->x2730);
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(s) : "f"(1.0f - c * c));
    half = -0.5f;
    sin60 = 0.8660254f;
    VSCALE(side, &obj->x26B0, s * sin60);
    VSCALE(up, &obj->x26C0, s);
    VSCALE(fwd, &obj->x26D0, c);
    VSCALE(down, &up, half);
    count = (int)(obj->x273C * obj->x2740 / obj->x272C) & 0xFC;
    if (i) {
        count = D_0029DCBC < count ? D_0029DCBC : count;
    } else {
        count = 0;
    }
    obj->x2690.i[3] = count;
    color = D_002694D0[obj->x281C];
    lo = obj->x2740;
    range = obj->x2744 - lo;
    VNEG(*pp, pp);
    pp->f[3] = 1.0f / obj->x27DA;

    VAdd(&b[0], &fwd, &up);
    VMadd(&b[0], pp, &b[0], lo + Rand01() * range);
    VAdd(&mid, &fwd, &down);
    VAdd(&b[42], &mid, &side);
    VMadd(&b[42], pp, &b[42], lo + Rand01() * range);
    VSub(&b[84], &mid, &side);
    VMadd(&b[84], pp, &b[84], lo + Rand01() * range);

    COPY_VECTOR(q, &obj->x26B0);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[0] = q;
    VSCALE(q, &obj->x26B0, half);
    VMadd(&q, &q, &obj->x26C0, -sin60);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[85] = q;
    VSCALE(q, &obj->x26B0, half);
    VMadd(&q, &q, &obj->x26C0, sin60);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[170] = q;

    a1[0].q = a1[85].q = a1[170].q = obj->x2690.q;
    colors[0] = colors[48] = colors[96] = color;
    obj->x2838[i] = obj->x2838[i + 42] = obj->x2838[i + 84] = obj->x27DA;
    __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%0)" : : "r"(&obj->x2690));
    n = i + 1;
    obj->x285D = n;
    obj->x2850 = (signed char)n;
    obj->x285E = n;
    obj->x285C = n;
}

void func_0023FAE8(Obj *obj, Vec *pos) {
    Vec p, fwd, up, side, down, mid, q;
    Vec *pp = &p;
    float c, s, lo, range;
    float half, sin60, ns;
    int i, k, n, count, j;
    Vec *a, *a1, *b;
    int *colors;
    signed char *pc;

    i = obj->x285C;
    p = *pos;
    a = &obj->x2830[i * 2];
    a1 = &obj->x2830[i * 2 + 1];
    b = &obj->x2834[i];
    colors = &obj->x283C[i];
    obj->x285A = 1;
    c = 1.0f - (obj->x2730 + obj->x2730);
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(s) : "f"(1.0f - c * c));
    half = -0.5f;
    sin60 = 0.8660254f;
    VSCALE(fwd, &obj->x2640, c);
    VSCALE(up, &obj->x2650, s);
    VSCALE(side, &obj->x2660, s * sin60);
    VSCALE(down, &up, half);
    count = (int)(obj->x273C * obj->x2740 / obj->x272C) & 0xFC;
    if (i) {
        if (D_0029DCBC < count)
            n = D_0029DCBC;
        else
            n = count;
    } else {
        n = 0;
    }
    obj->x2630.i[3] = n;
    lo = obj->x2740;
    range = obj->x2744 - lo;
    pp->f[3] = 1.0f / obj->x27DA;

    VAdd(&b[0], &fwd, &up);
    VMadd(&b[0], pp, &b[0], lo + Rand01() * range);
    VAdd(&mid, &fwd, &down);
    VAdd(&b[42], &mid, &side);
    VMadd(&b[42], pp, &b[42], lo + Rand01() * range);
    VSub(&b[84], &mid, &side);
    VMadd(&b[84], pp, &b[84], lo + Rand01() * range);

    COPY_VECTOR(q, &obj->x26D0);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[0] = q;
    ns = -sin60;
    VSCALE(q, &obj->x26C0, ns);
    VMadd(&q, &q, &obj->x26D0, -half);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[85] = q;
    VSCALE(q, &obj->x26C0, sin60);
    VMadd(&q, &q, &obj->x26D0, -half);
    q.f[2] = Rand01();
    q.f[3] = Rand01();
    a[170] = q;

    a1[0].q = a1[85].q = a1[170].q = obj->x2630.q;
    colors[0] = 0x80242460;
    colors[48] = 0x80246024;
    colors[96] = 0x80603030;
    obj->x2838[i] = obj->x2838[i + 42] = obj->x2838[i + 84] = obj->x27DA;
    __asm__ volatile ("lqc2 $vf30,0(%0)\n\tlqc2 $vf31,0(%0)" : : "r"(&obj->x2630));

    j = obj->x276E;
    pc = &obj->x285C;
    if (j >= 12) {
        int *tbl = D_0027F6C0;
        int k, idx;
        signed char *p;
        k = obj->x285C * 2 + 1;
        idx = k + tbl[0];
        p = &obj->x2830[idx].b[12];
        *p = 0;
        k = obj->x285D * 2 + 1;
        idx = k + tbl[1];
        p = &obj->x2830[idx].b[12];
        *p = 0;
        k = obj->x285E * 2 + 1;
        idx = k + tbl[2];
        p = &obj->x2830[idx].b[12];
        *p = 0;
    } else {
        int *tbl = D_0027F6C0;
        signed char *q;
        /* scheduling barrier: the retail code materialises the table address
           before the pointer copy, which the scheduler would otherwise swap */
        __asm__ volatile ("" : : : "memory");
        q = &obj->x285C;
        for (k = 2; k >= 0; k--) {
            if (*q <= 0) {
                int n2, idx2;
                signed char *p2;
                n2 = *q * 2 + 1;
                idx2 = n2 + *tbl;
                p2 = &obj->x2830[idx2].b[12];
                *p2 = 0;
            }
            tbl++;
            q++;
        }
    }
    {
        for (i = 0; i < 3; i++) {
            signed char ch = pc[i];
            if (ch < obj->x2854)
                pc[i] = ch + 1;
        }
    }
    {
        int m, d;
        d = obj->x285D;
        m = obj->x285E;
        if (d > m) m = d;
        obj->x2850 = m < obj->x285C ? obj->x285C : m;
    }
    obj->x276E = 0;
}
