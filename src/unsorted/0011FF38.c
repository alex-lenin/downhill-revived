/* UNIT 0x0011FF38 0x00120208 */
#include "particle_emission.h"
#if defined(__mips__)
#define CallShared00162690_0 func_00162690
#else
#define CallShared00162690_0 Forward00162690_0
#endif
#include "rider_geometry.h"
typedef struct { float x,y,z,w; } Vec;
typedef struct { char p0[0x16]; short x16; } Target;
typedef struct { void *x0; Target *x4; short x8,xA; } Part;
typedef struct { char p0[0x10]; void *x10,*x14; char p18[0x54]; Vec x6C; char p7c[0x80]; Part xFC; char p108[8]; char x110[0x50]; Vec x160,x170; char p180[0x20]; float x1A0,x1A4; char p1a8[0x60]; float x208,x20C; char p210[0x10]; float x220,x224,x228; char p22c[0x32]; short x25E; char p260[2]; short x262; char p264[4]; unsigned long x268; } Obj;
extern void func_001207F8(Obj*),func_00120CE8(Obj*),func_00236558(Vec*,Vec*,float),func_00203E10(Vec*,float),func_0011EDA8(Obj*),func_0011E568(Obj*,int),func_002182A8(void*,int),func_00162690(void*,float,int,void*);
extern float func_00238F40(void);extern float func_002223F0(void*,Target*);extern float func_00203C28(Vec*);
extern int func_00163D70(void*,Target*),func_00222BD8(int,int);
static inline float len(Vec*v) { float f; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(f):"f"(v->x),"f"(v->y),"f"(v->z));return f; }
static inline float clamp(float f) { float r,q; __asm__ volatile("min.s %0,%1,%2":"=&f"(r):"f"(f),"f"(1.0f)); __asm__ volatile("max.s %0,%1,%2":"=&f"(q):"f"(r),"f"(0.0f));return q; }
static inline int check(void *pos,Part *p) {
 if(p->xA==func_00163D70(p->x0,p->x4)) {if(func_002223F0(pos,p->x4)<5.0f)return 1;}
 if(p->x4->x16!=0)return 0;
 if(!(func_002223F0(pos,p->x4)<5.0f))return 0;
 return 1;
}
void func_0011FF38(Obj*o) {
 Vec tmp,copy; float speed,t,finalSpeed; unsigned long bit; int ok; void *pos; Part *part;
 if(o->x262)func_001207F8(o);
 else {
 speed=func_00260E08(o->x110, &o->x160, &o->x170, &tmp);
 func_00236558(&o->x160,&tmp,func_00238F40());
 bit=1; if(!(speed>5.0f))bit=0;
 o->x268=(o->x268&0xFFFBFFFFFFFFFFFFUL)|(bit<<50);
 func_00120CE8(o);
 t=clamp((len(&o->x170)-o->x208)/(o->x20C-o->x208));
 copy=o->x6C;func_00203E10(&copy,o->x224+t*(o->x228-o->x224));
 func_0011EDA8(o); pos=(char*)o->x10+0x10;part=&o->xFC;
 ok=check(pos,part);
 if((unsigned char)ok) {func_0011E568(o,9);func_002182A8(o->x10,0);if(o->x14)func_002182A8(o->x14,0);}
 }
 tmp=o->x6C; finalSpeed=func_00203C28(&tmp);
 if(finalSpeed<0.0f) {if(func_00222BD8(0,100)==42)o->x25E=10;}
 else if(finalSpeed<o->x220)o->x25E=10;
 o->x220=finalSpeed; o->x25E--; if(o->x25E<=0)o->x25E=0;
 CallShared00162690_0(&o->x1A0,o->x1A4,o->x25E!=0 && o->x25E<5,o->x10);
}
