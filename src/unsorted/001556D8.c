/* UNIT 0x001556D8 0x00155A20 */
#include "object_registry.h"
typedef struct Matrix { char pad0[0x40]; float x40[4],x50[4],x60[4],x70[4]; } Matrix;
typedef struct Rider {
 char pad0[0x4180]; float x4180[4]; char pad4190[0x244]; unsigned short x43D4;
 char pad43D6[0x7E]; unsigned short x4454; char pad4456[0xA76]; void *x4ECC;
 char pad4ED0[0x13E4]; char x62B4[1]; char pad62B5[0x1677]; Matrix *x792C;
 char pad7930[0x12C]; int x7A5C;
} Rider;
typedef struct Contact { char pad0[4]; unsigned short x4; char pad6[6]; int xC; char pad10[0x10]; float x20[4]; } Contact;
typedef struct State {
 Rider *x0; char pad4[0xC]; float x10,x14,x18; char pad1C[0x44]; float x60,x64,x68;
 char pad6C[8]; float x74; char pad78[0x10]; float x88,x8C; char pad90[0xC]; float x9C,xA0;
} State;
extern Contact *func_00138558(void *);
extern float func_001385A8(void *);

extern void func_00155E78(State *,int);
extern float func_00235EA8(float,float,float,float);
static inline float normalize(float *dst,float *src) {
 float x=src[0],y=src[1],z=src[2],length,reciprocal; int bits;
 /* Sony length/reciprocal wrapper, as in 00123DB0. */
 __asm__ volatile("mula.s %3,%3
	madda.s %4,%4
	madd.s %1,%5,%5
	sqrt.s %1,%1
	lui %2,0x3f80
	mtc1 %2,%0
	nop
	div.s %0,%0,%1" : "=&f"(reciprocal),"=&f"(length),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 dst[0]=x*reciprocal; dst[1]=y*reciprocal; dst[2]=z*reciprocal;
 return length;
}
static inline float dot3(float *a,float *b) { float r; __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(r) : "f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2])); return r; }
void func_001556D8(State *s,float *input) {
 int modeA=2,modeB=1;
 float scale,a1,t1,t2,t3,dot,x74,result,one;
 Contact *c;
 Matrix *m,*m2;
 float *cp,*mp,*mp2;
 float n[3];
 float length,dot2;
 Rider *r=s->x0;
 Rider *rb,*rc;
 if (r->x4ECC == r->x62B4) { modeA=7; modeB=7; }
 scale=100.0f;
 t1=input[0]*scale/s->x10; s->x60=t1;
 t2=input[1]*scale/s->x14; s->x64=t2;
 t3=input[2]*scale/s->x18; s->x68=t3;
 /* Zero-byte label (cf. 0010FF38): scheduling barrier so the compares stay below the stores. */
 __asm__ volatile ("Lt123_done:");
 a1=__builtin_fabsf(t1);
 /* Zero-byte label keeping t1 live past abs.s, so abs gets its own register ($f0, retail). */
 __asm__ volatile ("Lt1_live:" : : "f"(t1));
 if (a1 > scale || t2 < -100.0f || t2 > 400.0f || __builtin_fabsf(t3) > scale) {
  func_00155E78(s,modeA);
  return;
 }
 c=func_00138558((char *)r+0x40C0);
 if (c->xC != 0) return;
 if ((int)func_0017D3D0(c->x4) != 0) return;
 cp=c->x20;
 rb=s->x0;
 m=rb->x792C;
 mp=m->x60;
 __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(dot) : "f"(cp[0]),"f"(mp[0]),"f"(cp[1]),"f"(mp[1]),"f"(cp[2]),"f"(mp[2]));
 if (dot < -0.5f) { func_00155E78(s,modeA); return; }
 /* Zero-byte labels (cf. 001656A0): keep fabs result separate so it is abs+mov (retail). */
 {
  float at;
  __asm__ volatile ("Ldot_pre:" : "=f"(at));
  at=__builtin_fabsf(dot);
  __asm__ volatile ("Ldot_post:" : : "f"(at), "f"(dot));
  dot=at;
 }
 one=1.0f;
 __asm__ volatile ("Lone_pin:");
 dot=dot+dot*((one-func_001385A8((char *)rb+0x40C0))*0.5f);
 if (one < dot) dot=one;
 result=func_00235EA8(s->x9C,dot,0.2f,0.001f);
 s->x9C=result;
 x74=s->x74;
 if (x74 > result) {
  int first=(int)func_0017D3D0(s->x0->x43D4);
  int second=(int)func_0017D3D0(s->x0->x4454);
  if (first || second) return;
  func_00155E78(s,modeB);
 } else {
  rc=s->x0;
  length=normalize(n,rc->x4180);
  if (length > 20.0f) {
   m2=rc->x792C;
   mp2=m2->x40;
   dot2=dot3(n,mp2);
   {
    float at2;
    at2=__builtin_fabsf(dot2);
    /* Keeps dot2 live past abs.s: abs into a temp, then mov.s back (retail). */
    __asm__ volatile ("Ldot2_post:" : : "f"(dot2));
    dot2=at2;
    __asm__ volatile ("Ldot2_kill:" : "=f"(at2));
   }
   if (dot2 > 0.1f) {
    dot=result*(one-(dot2-0.1f));
    if (dot < x74) {
     int first=(int)func_0017D3D0(rc->x43D4);
     int second=(int)func_0017D3D0(s->x0->x4454);
     if (first || second) return;
     func_00155E78(s,modeB);
    }
   }
  }
 }
}
