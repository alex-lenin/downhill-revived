/* UNIT 0x00142D50 0x00143090 */
typedef unsigned int Quad __attribute__((mode(TI)));
typedef struct Vec { float x,y,z,w; } __attribute__((aligned(16))) Vec;
struct Body { Vec x0; char pad[0xE0]; struct Transform *xF0; };
struct Transform { char pad[0x10]; Vec x10; char p20[0x20]; Vec x40,x50,x60; };
extern void func_002226C8(Vec *, Vec *);
#define ADD(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*(dst)):"r"(a),"r"(b):"memory")
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*(dst)):"r"(a),"r"(b):"memory")
#define CROSS(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0":"=m"(*(dst)):"r"(a),"r"(b):"memory")
#define SCALE(dst,a,s) __asm__ volatile("lqc2 $vf11,0(%2)
	mfc1 $2,%3
	qmtc2 $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0":"=m"(*(dst)):"r"(dst),"r"(a),"f"(s):"$2")
#define MADD(dst,a,b,s) __asm__ volatile("lqc2 $vf11,0(%2)
	lqc2 $vf12,0(%3)
	mfc1 $2,%1
	qmtc2 $2,$vf13
	vaddax.xyz ACC,$vf11,$vf0x
	vmaddx.xyz $vf11,$vf12,$vf13x
	sqc2 $vf11,%0":"=m"(*(dst)):"f"(s),"r"(a),"r"(b),"r"(dst):"$2")
#define DOT(out,x,y,z) __asm__("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=&f"(out):"f"(x),"f"(y),"f"(z))
#define ROOT(out,v) __asm__ volatile("sqrt.s %0,%1":"=&f"(out):"f"(v))
static inline void NORMALIZE(Vec *p) {
 float x=p->x,y=p->y,z=p->z,d,r;
 DOT(d,x,y,z);
 __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(r):"f"(1.0f),"f"(d));
 p->x=x*r; p->y=y*r; p->z=z*r;
}
void func_00142D50(struct Body *obj) {
 Vec direction,delta,cross,matrix[4],savedX,savedZ;
 Vec *axis;
 int bits;
 float x,y,z,length,inverse,turn,cosine,d,planar,q;
 ADD(&obj->xF0->x10,&obj->xF0->x10,&obj->x0);
 x=obj->x0.x; y=obj->x0.y; z=obj->x0.z;
 __asm__ volatile ("mula.s %3,%3
	madda.s %4,%4
	madd.s %1,%5,%5
	sqrt.s %1,%1
	lui %2,0x3f80
	mtc1 %2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inverse), "=&f"(length), "=r"(bits) : "f"(x),"f"(y),"f"(z));
 direction.x=x*inverse; direction.y=y*inverse; direction.z=z*inverse;
 obj->x0.w=length;
 axis=&obj->xF0->x50;
 SUB(&delta,&direction,axis);
 CROSS(&cross,&direction,axis);
 d=delta.x*delta.x+delta.y*delta.y; ROOT(planar,d);
 planar=planar*length; turn=planar*16.0f;
 if(turn>1.0f) turn=1.0f;
 if(cross.z<0) turn=-turn;
 q=1.0f-turn*turn; ROOT(cosine,q);
 func_002226C8(matrix,&direction);
 *(Quad *)&savedX=*(Quad *)&matrix[0]; *(Quad *)&savedZ=*(Quad *)&matrix[2];
 SCALE(&delta,&savedX,cosine); MADD(&matrix[0],&delta,&savedZ,-turn);
 SCALE(&delta,&savedZ,cosine); MADD(&matrix[2],&delta,&savedX,turn);
 SCALE(&savedX,&matrix[0],0.0625f); MADD(&obj->xF0->x40,&savedX,&obj->xF0->x40,0.9980449676513672f);
 SCALE(&savedX,&matrix[1],0.9980449676513672f); MADD(&obj->xF0->x50,&savedX,&obj->xF0->x50,0.0625f);
 SCALE(&savedZ,&matrix[2],0.0625f); MADD(&obj->xF0->x60,&savedZ,&obj->xF0->x60,0.9980449676513672f);
 NORMALIZE(&obj->xF0->x50); CROSS(&obj->xF0->x40,&obj->xF0->x50,&obj->xF0->x60);
 NORMALIZE(&obj->xF0->x40); CROSS(&obj->xF0->x60,&obj->xF0->x40,&obj->xF0->x50);
}
