/* UNIT 0x0015A608 0x0015A7C0 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Link { void *x0; Vec *x4; short x8,xA; };
struct State { char pad0[0x40]; struct Link *x40; };
struct Body { unsigned long flags; char pad8[8]; Vec pos; };
struct Obj { char pad0[8]; struct Body *x8,*xC; char pad10[0xC8]; struct Link xD8; char padE4[0x1C]; Vec x100; char pad110[0x20]; struct Link *x130; char pad134[12]; float x140,x144; char pad148[8]; Vec x150,x160; char pad170[0x20]; Vec x190,x1A0; char pad1B0[0x3C]; short x1EC; };
extern char D_003041B0[];
extern Vec *func_00163D40(void *,int);
extern void func_002609B0(void *);
#if defined(__mips__)
extern float func_00211980(Vec *,Vec *,int,float);
#endif
extern void func_0015B558(struct Obj *,Vec *,Vec *,int);
extern void func_0015BAD8(struct Obj *,Vec *,Vec *);
extern void func_002182A8(struct Body *,int);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void copy3(Vec *dst,Vec *src) { float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w; }
static inline float dot(Vec *a,Vec *b) { float r; __asm__("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r; }
void func_0015A608(struct Obj *o,Vec *arg) {
 Vec a,b,c,d; Vec *p,*q; struct Link *l; struct State *st;
 if(o->x8) {
 l=&o->xD8; o->xD8.x0=D_003041B0; p=arg; if(!arg) p=func_00163D40(D_003041B0,0);
 l->x4=p; l->xA=-1; l->x8=-1; st=(struct State *)((char *)o+0xF0); st->x40=l; func_002609B0(st);
 copy3(&a,&o->x100); COPY_VECTOR(b,arg);
 b.z-=CallGroundProbe(&b,&d,0,15.0f);
 q=&o->x1A0; COPY_VECTOR(*q,&d); o->x1A0.w=dot(&d,&b); COPY_VECTOR(o->x190,q);
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(c));
 o->x140=b.x; o->x144=b.y; copy3(&o->x150,&c); copy3(&o->x160,&a);
 func_0015B558(o,&a,&d,1); COPY_VECTOR(o->x8->pos,&b);
 if(o->xC) { COPY_VECTOR(o->xC->pos,&b); func_0015BAD8(o,&a,&d); }
 o->x8->flags|=((unsigned long)0x8000<<42); o->x1EC=0;
 func_002182A8(o->x8,1); func_002182A8(o->xC,1);
 }
}
