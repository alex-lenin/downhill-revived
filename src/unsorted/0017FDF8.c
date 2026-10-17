/* UNIT 0x0017FDF8 0x0017FFB8 */
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Vec {float x,y,z,w;} Vec __attribute__((aligned(16)));
typedef struct Mtx {Vec r[4];} Mtx;
typedef struct Body {char p0[16]; Vec pos; char p20[32]; Mtx matrix;} Body;
typedef struct Ref {void *x0; Vec *x4; short x8,xA;} Ref;
typedef struct Work {char p0[16]; float x10[4]; char p20[32]; Ref *x40;} Work;
typedef struct Obj {
 char p0[8]; Body *x8,*xC; char p10[0xD8-0x10]; Ref ref;
 char pE4[12]; Work work;
 char p134[12]; float x140,x144; char p148[8]; Vec x150,x160; char p170[16]; Vec x180,x190;
 char p1a0[8]; int x1A8;
} Obj;
extern char D_003BD580[];
extern Vec *func_00163D40(void *,int);
extern void func_002609B0(void *);
#if defined(__mips__)
extern float func_00211980(Vec *,Vec *,int,float);
#endif
extern void func_00181228(Obj *,Vec *,Vec *,int);
extern void func_002182A8(Body *,int);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_MATRIX(dst,src) __asm__ volatile("lq $8,0(%4)\n\tlq $9,16(%4)\n\tlq $10,32(%4)\n\tlq $11,48(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3" : "=m"((dst).r[0]),"=m"((dst).r[1]),"=m"((dst).r[2]),"=m"((dst).r[3]) : "r"(src) : "$8","$9","$10","$11")
static __inline__ void copy3(Vec *dst,Vec *src) { float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w; }
static __inline__ float dot(Vec *a,Vec *b) {float r; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r;}
void func_0017FDF8(Obj *obj,Vec *arg1) {
 Vec initial,pos,zero,normal;
 Ref *ref; Vec *value,*plane,*out; Work *work;
 if(obj->x8) {
 ref=&obj->ref;ref->x0=D_003BD580;
 value=arg1 ? arg1 : func_00163D40(D_003BD580,0);
 ref->x4=value;ref->xA=-1;work=&obj->work;ref->x8=-1;work->x40=ref;
 func_002609B0(work);
 copy3(&initial,(Vec *)obj->work.x10);
 COPY_VECTOR(pos,arg1);
 pos.z-=CallGroundProbe(&pos,&normal,0,5.0f);
 plane=&obj->x190;COPY_VECTOR(*plane,&normal);
 obj->x190.w=dot(&normal,&pos);
 out=&obj->x180;COPY_VECTOR(*out,plane);
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(zero));
 obj->x140=pos.x;obj->x144=pos.y;
 copy3(&obj->x150,&zero);copy3(&obj->x160,&initial);
 obj->x1A8=0;
 func_00181228(obj,&initial,out,1);
 COPY_VECTOR(obj->x8->pos,&pos);
 if(obj->xC) {COPY_VECTOR(obj->xC->pos,&obj->x8->pos);COPY_MATRIX(obj->xC->matrix,&obj->x8->matrix);}
 func_002182A8(obj->x8,1);func_002182A8(obj->xC,1);
 }
}
