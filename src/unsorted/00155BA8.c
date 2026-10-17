/* UNIT 0x00155BA8 0x00155E70 */
#include "object_registry.h"
typedef unsigned int Vec __attribute__((mode(TI)));
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
extern float func_001385A8(void *,Rider *),func_0012DF48(Rider *);

extern void func_00155E78(State *,int);
extern float func_00235EA8(float,float,float,float),func_00236088(float,float,float,float);
extern void func_00221888(Vec *,float *,float *);
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
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
typedef struct Transform { float rows[4][4]; } Transform __attribute__((aligned(16)));
void func_00155BA8(State *s,float *input) {
 float normalized[3]; Transform matrix; Transform *transform; float angle;
 float blend,length,reciprocal,x,y,z,dot,limit,minimum;
 Rider *r,*r2; Matrix *m; float *forward,*velocity; Contact *contact; int first,second,mode;
 if(func_0012DF48(s->x0)!=0.0f) s->x8C=1.0f;
 else s->x8C=func_00236088(s->x8C,s->x88,0.001f,0.01f);
 r=s->x0; blend=s->x8C;
 length=normalize(normalized,r->x4180);
 if(!(length>20.0f)) return;
 {
  first=(int)func_0017D3D0(r->x43D4); second=(int)func_0017D3D0(s->x0->x4454);
  if(first || second) return;
  {
   r2=s->x0; m=r2->x792C; forward=m->x50; transform=(Transform *)m->x40;
   mode=2; if(r2->x4ECC==r2->x62B4) mode=7;
   __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(dot) : "f"(normalized[0]),"f"(forward[0]),"f"(normalized[1]),"f"(forward[1]),"f"(normalized[2]),"f"(forward[2]));
   if(dot < 1.0f-blend*0.5f) {
    /* Sony sceVu0CopyMatrix, as used by promoted siblings. */
    __asm__ volatile("lq $8,0(%4)
	lq $9,16(%4)
	lq $10,32(%4)
	lq $11,48(%4)
	sq $8,%0
	sq $9,%1
	sq $10,%2
	sq $11,%3" : "=m"(matrix.rows[0]),"=m"(matrix.rows[1]),"=m"(matrix.rows[2]),"=m"(matrix.rows[3]) : "r"(transform) : "$8","$9","$10","$11");
    contact=func_00138558((char *)r2+0x40C0);
    COPY_VECTOR(matrix.rows[2],contact->x20);
    __asm__ volatile("lqc2 $vf11, 0(%3)
	lqc2 $vf12, 0(%4)
	vopmula.xyz ACC, $vf11, $vf12
	vopmsub.xyz $vf11, $vf12, $vf11
	sqc2 $vf11, %0
	lqc2 $vf11, 0(%4)
	lqc2 $vf12, 0(%2)
	vopmula.xyz ACC, $vf11, $vf12
	vopmsub.xyz $vf11, $vf12, $vf11
	sqc2 $vf11, %1" : "=m"(matrix.rows[0]),"=m"(matrix.rows[1]) : "r"(&matrix.rows[0]),"r"(&matrix.rows[1]),"r"(&matrix.rows[2]) : "memory");
    func_00221888((Vec *)matrix.rows,normalized,&angle);
    limit=blend;
    if(s->x0->x7A5C==3) limit+=s->xA0*0.2f;
    if(input[2]>0.3f) {
     minimum=blend*0.7f;
     limit*=1.0f-(input[2]-0.3f);
     if(limit<minimum) limit=minimum;
    }
    if(limit<angle) func_00155E78(s,mode);
    else if(angle < -limit) func_00155E78(s,mode);
   }
  }
 }
}
