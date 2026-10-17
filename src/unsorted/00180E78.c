/* UNIT 0x00180E78 0x00181070 */
typedef struct Vec { float x,y,z,w; } __attribute__((aligned(16))) Vec;
typedef struct Matrix { Vec v[4]; } Matrix;
typedef struct Frame { char pad0[16]; Vec pos; char pad20[32]; Matrix matrix; } Frame;
typedef struct Obj { char pad0[8]; Frame *x8,*xC; char pad10[0x130]; Vec x140,x150,x160,x170,x180,x190; char pad1A0[8]; float x1A8; } Obj;
extern int func_001812F8(Obj *);
extern float func_00238F40(void);
extern float func_00181070(Obj *,Vec *,int);
extern int func_00181228(Obj *,Vec *,Vec *,int);
extern int func_00211280(Frame *);
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst): "r"(src): "$2")
static inline void addscale(Vec *out,Vec *a,Vec *b,float s) {
__asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out):"r"(a),"r"(b),"f"(s): "$2");
}
static inline void sub(Vec *out,Vec *a,Vec *b) {
__asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out):"r"(a),"r"(b));
}
void func_00180E78(Obj *self,int mode) {
Vec pos,dir; Vec *range=&self->x190,*direction=&self->x150,*saved=&self->x140; float maxdrop,s,len,inv,x,y,z,drop,newz;
func_001812F8(self);
maxdrop=func_00238F40()*32.0f;
s=range->z*func_00238F40();
addscale(&pos,&self->x8->pos,direction,s);
pos.z=self->x8->pos.z+self->x1A8*func_00238F40();
sub(&dir,&pos,&self->x8->pos);
x=dir.x; y=dir.y; z=dir.z;
__asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(len):"f"(x),"f"(y),"f"(z));
__asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv):"f"(1.0f),"f"(len));
dir.x=x*inv; dir.y=y*inv; dir.z=z*inv;
if(len<0.001f) { COPY_VECTOR(dir,&self->x160); }
s=func_00181070(self,&pos,mode!=0);
__asm__ volatile("min.s %0,%1,%2" : "=&f"(drop):"f"(s),"f"(maxdrop));
pos.z-=drop;
self->x1A8-=drop;
__asm__ volatile("max.s %0,%1,%2" : "=&f"(newz):"f"(-117.33333587646484f),"f"(self->x1A8));
self->x1A8=newz;
func_00181228(self,&dir,&self->x180,mode);
COPY_VECTOR(self->x8->pos,&pos);
saved->x=self->x8->pos.x;
saved->y=self->x8->pos.y;
func_00211280(self->x8);
if(self->xC) {
COPY_VECTOR(self->xC->pos,&self->x8->pos);
__asm__ volatile("lq $8,0(%4)
	lq $9,16(%4)
	lq $10,32(%4)
	lq $11,48(%4)
	sq $8,%0
	sq $9,%1
	sq $10,%2
	sq $11,%3" : "=m"(self->xC->matrix.v[0]),"=m"(self->xC->matrix.v[1]),"=m"(self->xC->matrix.v[2]),"=m"(self->xC->matrix.v[3]): "r"(&self->x8->matrix): "$8","$9","$10","$11");
}
}
