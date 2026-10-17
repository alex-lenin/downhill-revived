/* UNIT 0x00161B10 0x00162688 .sdata=0x29C7C0:0x29C7E0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00239540_10_func_00239540 func_00239540
#else
#define CallScalarBridge00239540_10_func_00239540 ScalarBridge00239540_10
#endif
#include "particle.h"
/* One TU with func_00162520 (src/unsorted/00162520.c): D_0029C7CC and D_0029C7D8 are $gp
   outside delay slots here, D_0029C7D4 there, so the sdata must come from one file. */
#include "include_asm.h"

typedef struct { float x, y, z, w; } Vec __attribute__((aligned(16)));

typedef struct Effect {
    char pad0[0x2720];
    unsigned int x2720;
    unsigned short x2724, x2726;
    char pad2728[0x4];
    float x272C;
    char pad2730[0x10];
    float x2740;
    float x2744;
    char pad2748[0x10];
    unsigned int x2758;
    char pad275C[0x10];
    unsigned short x276C;
    unsigned short x276E;
    char pad2770[0x50];
    unsigned int x27C0;
    char pad27C4[0x4];
    float x27C8;
    float x27CC;
    char pad27D0[0x4];
    float x27D4;
    char pad27D8[0x50];
    int x2828;
    char pad282C[0x1C];
    struct Effect **x2848;
} Effect;

typedef struct Obj {
    char pad0[0x3FD0];
    char x3FD0[0x3750];
    char x7720[0x20C];
    char *x792C;
    char pad7930[0x11C];
    Effect *x7A4C;
} Obj;

typedef struct {
    unsigned int flags;
    unsigned short x4, x6;
    char pad8[0x4];
    float xC;
    char pad10[0x4];
    Vec *x14;
    char pad18[0x8];
    float x20;
    float x24;
    char pad28[0x10];
    unsigned int x38;
    char pad3C[0x4];
} Entry;

typedef struct {
    char pad0[0x8];
    float x8;
    float xC;
    char pad10[0x4];
    float x14;
    char pad18[0x38];
} Scale;

float D_0029C7C0 = 0.0625f;
float D_0029C7C4 = 0.375f;
float D_0029C7C8 = 0.75f;
float D_0029C7CC = 0.00390625f;
float D_0029C7D0 = 10.0f;
int D_0029C7D4 = 30;
unsigned int D_0029C7D8 = 0x12345678;

extern "C" Entry D_0027BFC0[];
extern "C" Scale D_0027D300[];
extern "C" Vec D_0026E950;
extern "C" void func_0022DDD8(Effect *);
extern "C" Effect *func_0022DD88(int);
extern "C" void func_001C7CB8(void *);
extern "C" void func_0012D838(Obj *, Effect *, Effect *);
extern "C" void func_00155E78(void *, int);

extern "C" void func_00161B10(Obj *obj, int index, float strength, int arg2)
{
    Effect *effect;
    Vec pos;
    float scale;
    float t;

    if (!(D_0029C7CC < strength) && !(D_0027BFC0[index].flags & 0x4000000)) {
        return;
    }
    effect = obj->x7A4C;
    if (effect) {
        if (effect->x2758 < D_0027BFC0[index].x38) {
            func_0022DDD8(effect);
            obj->x7A4C = 0;
            effect = 0;
        }
    }
    if (!effect) {
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                          : "=m"(pos) : "r"(D_0027BFC0[index].x14), "r"(&D_0026E950));
        effect = func_0022DD88(func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, index, (Vector4 *)(&pos), 1.0f, (Matrix4 *)(0), (int)(0), (int)(arg2)));
        if (!effect) {
            return;
        }
        obj->x7A4C = effect;
        effect->x2848 = &obj->x7A4C;
    }
    if (effect->x276C != index) {
        return;
    }
    effect->x2726 = D_0027BFC0[index].x6;
    effect->x2828 = arg2;
    if (effect->x27C0 & 0x70000) {
        __asm__ volatile ("sqrt.s %0, %1" : "=f"(scale) : "f"(strength));
        /* clamp to [0.5, 4]: g++ <? / >? operators give min.s/max.s */
        {
            float hi = 4.0f;
            float lo = 0.5f;
            scale = hi <? scale;
            scale = lo >? scale;
        }
        if (effect->x27C0 & 0x10000) {
            effect->x27C8 = D_0027D300[index].x8 * scale;
            effect->x27CC = D_0027D300[index].xC * scale;
            effect->x27D4 = D_0027D300[index].x14 * scale;
        }
        if (effect->x27C0 & 0x20000) {
            t = strength + strength;
            if (effect->x27C0 & 0x120000) {
                t = strength / effect->x27CC;
            }
            if (effect->x2720 & 0x4000000) {
                if (t < 1.0f) {
                    t = 1.0f;
                }
            }
            effect->x272C = D_0027BFC0[index].xC * t;
        }
        if (effect->x27C0 & 0x40000) {
            effect->x2740 = D_0027BFC0[index].x20 * scale;
            effect->x2744 = D_0027BFC0[index].x24 * scale;
        }
    }
    if (index == 2) {
        if (++effect->x276E == 0x79) {
            Effect *first = 0;
            Effect *second;
            int a = func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 0x41, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
            int b = func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 0x42, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)(0));
            if (a != -1) {
                first = func_0022DD88(a);
            }
            second = 0;
            if (b != -1) {
                second = func_0022DD88(b);
            }
            func_001C7CB8(obj->x7720);
            func_0012D838(obj, first, second);
        } else {
            if (effect->x276E == 0xF0) {
                func_00155E78(obj->x3FD0, 10);
            }
            D_0029C7D8 = D_0029C7D8 * 0x41C64E6D + 0x3039;
            if (((D_0029C7D8 >> 16) & 0x1F) == 0) {
                func_0022CDC0((Vector4 *)(obj->x792C + 0x10), 0x19, (Matrix4 *)(0), (int)(0), 1.0f, (Vector4 *)(0));
            }
        }
    }
}

typedef unsigned int Quad __attribute__((mode(TI)));
typedef struct BikeState {
 char pad0[0x34]; Vec *x34; Vec *x38;
 char pad3C[0x2E0]; int x31C; Vec x320; Vec x330;
 char pad340[0x14]; unsigned char x354;
 char pad355[0xDF]; float x434;
} BikeState;
typedef struct TrailEffect {
 char pad0[0x2630]; Vec x2630;
 char pad2640[0x40]; Vec x2680;
 char pad2690[0x10]; Vec x26A0;
 char pad26B0[0x50]; Vec x2700;
 char pad2710[0x10]; unsigned int x2720; short x2724;
 char pad2726[6]; float x272C;
 char pad2730[0xEC]; unsigned int x281C;
} TrailEffect;
typedef struct Rider {
 char pad0[0x40C0]; char x40C0[0x438];
 char pad44F8[0x9D4]; void *x4ECC;
 char pad4ED0[0x13E4]; char x62B4[0x28]; int x62DC;
 char pad62E0[0x330]; char x6610[0x12F8]; int x7908;
 char pad790C[0x78]; int x7984; void *x7988;
 char pad798C[0xB8]; TrailEffect *x7A44; void *x7A48;
 Effect *x7A4C; Effect *x7A50;
 char pad7A54[8]; int x7A5C; char pad7A60[0x30]; Vec x7A90;
} Rider;
typedef struct { int x0; short x4; unsigned short flags; } TrailSurface;
extern "C" unsigned long *func_00138558(void *);
extern "C" int func_00138560(void *);
extern "C" unsigned int func_00161868(int, void *);
extern "C" void func_001619B0(void *);
extern "C" int func_001E64C8(void *);
extern "C" void func_00239778(void *);
extern "C" void func_00212EA8(Vec *, Vec *, void *, int);

#if defined(__mips__)
extern "C" void func_00239540(void *,Vec *,Vec *,Vec *,Vec *,unsigned int,long);
#else

#endif

extern "C" int func_00238F60();
extern "C" void func_0023EE58(Effect **,int,Vec *,Vec *,Vec *,unsigned int);
extern "C" unsigned char D_0026E990[];
extern "C" int D_0039E830[], D_004B2828[];
extern "C" float D_0027C40C[];
extern "C" char D_0038DB90[];
static inline void vsub(Vec *out, Vec *a, Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"r"(b)); }
#define VSUB(out,a,b) vsub(&out,a,b)
static inline void vadd(Vec *out, Vec *a, Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"r"(b)); }
#define VADD(out,a,b) vadd(&out,a,b)
static inline void vscale(Vec *out, Vec *a, float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"f"(f):"$2"); }
#define VSCALE(out,a,f) vscale(&out,a,f)
static inline void vmadd(Vec *out, Vec *a, Vec *b, float f) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"r"(b),"f"(f):"$2"); }
#define VMADD(out,a,b,f) vmadd(&out,a,b,f)
static inline float trailDot(Vec *a, Vec *b) {
 float result;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(result):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return result;
}
static inline float trailSquared(Vec *v) {
 float sum;
 __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3":"=f"(sum):"f"(v->x),"f"(v->y),"f"(v->z));
 return sum;
}
static inline float trailLength(Vec *v) {
 float speed;
 __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0":"=&f"(speed):"f"(v->x),"f"(v->y),"f"(v->z));
 return speed;
}
struct Part { char pad[0x28]; int x28; };
static inline int partState(Part *part) { return part->x28; }
static inline int trailBelow(float value, float limit) { return value < limit; }
static inline int trailNotEqual(int a, int b) { return a != b; }
static inline int trailMax(int a, int b) { return a < b ? b : a; }
extern "C" void func_00161EB8(Rider *obj) {
 TrailEffect *effect = obj->x7A44;
 BikeState *bike = (BikeState *)obj->x40C0;
 Vec transformed;
 if (effect) {
  int active = 0;
  int surface, index;
  TrailSurface *entry;
  float speed;
  *(Quad *)&effect->x2680 = *(Quad *)&effect->x2630;
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(transformed):"r"(&D_0026E950),"r"(bike->x38));
  VADD(effect->x2630,&transformed,bike->x34);
  surface = (unsigned short)(*func_00138558(bike) >> 53) & 63;
  if (surface >= 32) surface = 0;
  entry = (TrailSurface *)(surface * 8 + D_0038DB90);
  effect->x281C = func_00161868(entry->x0,&obj->x7A90);
  VSUB(effect->x26A0,(&effect->x2630),(&effect->x2680));
  speed = trailLength(&effect->x26A0);
  effect->x26A0.w = speed;
  if (speed > 3.0f) effect->x2724 = 1;
  index = entry->x4;
  effect->x272C = 0;
  if (!effect->x2724) {
   VSCALE(effect->x2700,bike->x38+2,D_0026E950.w);
   if (!partState((Part *)obj->x62B4)) effect->x2720 |= 0x4000;
   else effect->x2720 &= ~0x4000;
   if (D_0039E830[0] && (obj->x7A5C ^ 3) != 0 && obj->x7984 && func_00138560(bike) && func_001E64C8(obj->x6610) && D_004B2828[0] < 3) {
    func_001619B0(obj);
    func_00239778(obj->x7988);
   }
   if (!func_00138560(bike)) {
    if (D_0026E990[index+1] && index != 45 && index != 8 && obj->x4ECC != obj->x62B4) {
     float strength = effect->x26A0.w;
     func_00161B10((Obj *)obj,index == 55 ? 2 : index,strength,effect->x281C);
    }
    if (effect->x26A0.w > D_0029C7CC) {
     int fast = bike->x434 > D_0029C7D0 && obj->x4ECC != obj->x62B4;
     if (fast) effect->x2720 |= 0x400000;
     else effect->x2720 &= ~0x400000;
     if (((TrailSurface *)(surface*8+D_0038DB90))->flags & 1)
      effect->x272C = D_0027C40C[0] * effect->x26A0.w;
     active = fast ^ 1;
    }
   }
   void *trail = obj->x7A48;
   if (trail) {
    int valid = (((TrailSurface *)(surface*8+D_0038DB90))->flags & 4) && !bike->x31C && obj->x7908 < 35;
    Vec target, delta, end;
    Vec collision[3];
    active &= valid;
    *(Quad *)&target = 0;
    Vec *normal = &bike->x330;
    if (active) {
     if (bike->x354 == 0) {
      VMADD(delta,(&effect->x2630),bike->x38+2,3.0f);
      VSUB(end,(&effect->x2630),bike->x38+2);
      func_00212EA8(&delta,&end,collision,0);
      VMADD(target,&collision[1],bike->x38+2,D_0029C7C0);
     } else if (bike->x354 == 1) {
      float distance, dot;
      VSUB(delta,(&effect->x2630),&bike->x320);
      distance = trailDot(normal,&delta);
      dot = trailDot(normal,bike->x38+2);
      if (dot > 0.5f && distance < 0.25f) {
       VMADD(target,(&effect->x2630),bike->x38+2,D_0029C7C0-distance/dot);
      } else active = 0;
     } else active = 0;
     if (active) {
      VSUB(delta,(&effect->x2630),&target);
      active = trailBelow(trailSquared(&delta),100.0f);
     }
    }
    unsigned int flags = (effect->x281C >> 25)|0x80;
    Vec *axis = bike->x38+1;
    long display = 0;
    if (active && index != 8 && index != 45) display = trailNotEqual(index,55);
    CallScalarBridge00239540_10_func_00239540(trail,&target,axis,(&effect->x26A0),normal,flags,display);
    if (active && (index == 8 || index == 45 || index == 55)) {
     Effect *other = obj->x7A50;
     if (other && other->x276C != index) {
      if (D_0027BFC0[index].x38 < other->x2758) return;
      func_0022DDD8(other);
      obj->x7A50 = 0;
     }
     int count = func_00238F60();
     count = trailMax(1,count);
     VSUB(delta,(&effect->x2680),(&effect->x2630));
     VSCALE(delta,&delta,1.0f/count);
     *(Quad *)&end = *(Quad *)&bike->x330;
     func_0023EE58(&obj->x7A50,index,&target,&end,&delta,effect->x281C);
    }
   }
  } else effect->x2724--;
 }
}


typedef struct Effect2 {
 char pad0[0x2720]; unsigned int x2720; unsigned short x2724, x2726;
 char pad2728[0x30]; unsigned int x2758; char pad275C[0x10]; unsigned short x276C;
 char pad276E[0x8A]; int x27F8; char pad27FC[0x20]; unsigned int x281C;
 char pad2820[0x28]; struct Effect2 **x2848; char pad284C[0xC]; unsigned short x2858;
} Effect2;
typedef struct Obj2 {
 char pad0[0x40C0]; char x40C0[0x386C]; char *x792C;
 char pad7930[0x114]; Effect2 *x7A44; char pad7A48[8]; Effect2 *x7A50;
} Obj2;
typedef struct { char pad[6]; unsigned short flags; } Surface;
typedef struct { char pad[0x280]; unsigned int x280; unsigned short x284, x286; } Settings;
extern "C" unsigned int D_0027C278[];

extern "C" unsigned long *func_00138558(void *);
#define SETTINGS (*(Settings *)D_0027BFC0)
extern "C" void func_00162520(Obj2 *obj) {
 Effect2 *effect = obj->x7A50;
 Effect2 *other = obj->x7A44;
 int index;
 if (effect) {
  if (effect->x276C != 10) {
   if (D_0027C278[0] < effect->x2758) return;
   func_0022DDD8((Effect *)effect);
   effect = 0;
   obj->x7A50 = 0;
  }
 }
 if (!effect) {
  index = (unsigned short)(*func_00138558(obj->x40C0) >> 53) & 63;
  if (index >= 32) index = 0;
  if (((Surface *)(index * 8 + (char *)D_0038DB90))->flags & 8) {
   effect = (Effect2 *)func_0022DD88(func_0022CFB8((Vector4 *)(obj->x792C + 0x10), obj->x792C + 0x40, 10, (Vector4 *)(0), 1.0f, (Matrix4 *)(0), (int)(0), (int)((other->x281C >> 1) & 0x7F7F7F)));
   if (!effect) return;
   obj->x7A50 = effect;
   effect->x27F8 = D_0029C7D4;
   obj->x7A50->x2848 = &obj->x7A50;
  }
 }
 if (effect) {
  if (effect->x27F8 > 0) effect->x27F8--;
  else {
   effect->x27F8 = D_0029C7D4;
   effect->x2720 = SETTINGS.x280 | 0x800;
   effect->x2726 = SETTINGS.x286;
   effect->x2724 = SETTINGS.x284;
   effect->x2858 = 15;
  }
 }
}
