/* UNIT 0x001101B8 0x00110330 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Frame {char pad[16];Vec x10;char pad20[32];Vec x40;} Frame;
typedef struct Entity {char pad[0x792c];Frame *x792C;} Entity;
typedef struct Obj {Entity *x0;char pad4[12];Vec x10;char pad20[16];Vec x30;char pad40[48];int x70;char pad74[124];Vec xF0,x100;char pad110[208];Vec x1E0;char pad1F0[80];float x240;char pad244[44];float x270;char pad274[8];float x27C;char pad280[40];float x2A8;} Obj;
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
static inline void Scale(Vec *d,Vec *s,float f) {
 __asm__ volatile("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0":"=m"(*d):"r"(s),"f"(f):"$2");
}
static inline void Add(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*d):"r"(a),"r"(b));
}
static inline void Sub(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*d):"r"(a),"r"(b));
}
static inline float Normalize(Vec *out, Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 int bits;
 __asm__ volatile("mula.s %3,%3
	madda.s %4,%4
	madd.s %1,%5,%5
	sqrt.s %1,%1
	"
  "lui %2,0x3f80
	mtc1 %2,%0
	nop
	div.s %0,%0,%1"
  :"=&f"(inv),"=&f"(len),"=r"(bits):"f"(x),"f"(y),"f"(z));
 out->x=x*inv;
 out->y=y*inv;
 out->z=z*inv;
 return len;
}
void func_001101B8(Obj *obj,float t) {
 Vec pos;
 Vec *v;
 float dot,mag;
 if(t<obj->x240) {
  t=obj->x240-t;
  if(obj->x270<t) {
   if(obj->x70) COPY_VECTOR(pos,&obj->x10);
   else COPY_VECTOR(pos,&obj->x30);
  } else {
   Scale(&pos,&obj->xF0,-t);
   Add(&pos,&pos,&obj->x30);
  }
 } else {
  t-=obj->x240;
  Scale(&pos,&obj->x100,t);
  Add(&pos,&pos,&obj->x30);
 }
 v=&obj->x1E0;
 Sub(v,&pos,&obj->x0->x792C->x10);
 obj->x2A8=Normalize(v,v);
 v=&obj->x1E0;
 { Vec *forward=&obj->x0->x792C->x40;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(dot):"f"(forward->x),"f"(v->x),"f"(forward->y),"f"(v->y),"f"(forward->z),"f"(v->z));
 }
 obj->x27C=dot;
 __asm__ volatile("L1101b8_a:");
 mag=__builtin_fabsf(dot);
 __asm__ volatile("L1101b8_b:" : : "f"(dot), "f"(dot));
 if(mag<0.053f) obj->x27C=0.0f;
}
