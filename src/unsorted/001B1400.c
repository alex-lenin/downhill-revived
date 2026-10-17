/* UNIT 0x001B1400 0x001B1698 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Obj { char pad[0xa0]; Vec xA0,xB0,xC0; char pD0[12]; unsigned char xDC; char pDD[3]; int xE0,xE4,xE8; char pEC[4]; Vec xF0; char p100[4]; float x104,x108; } Obj;
extern Vec D_007684B0[];
extern int D_0039E82C[];
#define SUB(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(d):"r"(a),"r"(b))
#define CROSS(d,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(d):"r"(a),"r"(b))
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0":"=m"(dst):"r"(src):"$2")
#define NORMALIZE(v,l,i) __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1":"=&f"(i),"=&f"(l):"f"((v).x),"f"((v).y),"f"((v).z),"f"((v).z):"$2")
#define SQRT(d,s) __asm__("sqrt.s %0,%1":"=&f"(d):"f"(s))
static inline float Normalize(Vec *v) {
 float len,recip;
 NORMALIZE((*v),len,recip);
 v->x*=recip; v->y*=recip; v->z*=recip; return len;
}
static inline float IntToFloat(int value) { return (float)value; }
void func_001B1400(Obj *obj) {
 Vec cross,dir,side,unused,delta; Vec *base=D_007684B0,*a,*b,*out,*origin,*start; float length,inv,norm,r1,r2,scale,sum,y,z;
 b=&base[obj->xE4]; a=&base[obj->xE0]; SUB(dir,b,a);
 length=Normalize(&dir);
 SUB(delta,&base[obj->xE8],a);
 if(obj->xDC) { CROSS(cross,&dir,&delta); } else { CROSS(cross,&delta,&dir); }
 Normalize(&cross);
 COPY_VECTOR(obj->xF0,&cross);
 if(obj->xDC) { CROSS(side,&cross,&dir); } else { CROSS(side,&dir,&cross); }
 origin=&obj->xA0; start=(Vec*)obj->xE0; start=(Vec*)((int)start<<4); start=(Vec*)((int)start+(int)base); COPY_VECTOR(obj->xA0,start);
 COPY_VECTOR(obj->xC0,(Vec*)((obj->xE4<<4)+(int)base));
 scale=IntToFloat(D_0039E82C[0]!=0)+1.0f;
 r1=obj->x104*scale; r2=obj->x108*scale; sum=r1+r2;
 out=&obj->xB0;
 if(sum<length) {
 Vec *end;
 end=(Vec*)((obj->xE4<<4)+(int)base); start=(Vec*)obj->xE0; start=(Vec*)((int)start<<4); start=(Vec*)((int)start+(int)base);
 scale=r1/sum;
 SUB(obj->xB0,end,start);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0":"=m"(obj->xB0):"r"(out),"f"(scale):"$2","memory");
 } else {
 obj->xB0.x=0;
 y=(r1*r1-r2*r2+length*length)/(2.0f*length);
 out->y=y;
 SQRT(z,r1*r1-y*y); out->z=z;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,%2\n\tlqc2 $vf13,%3\n\tlqc2 $vf14,%4\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0":"=m"(obj->xB0):"r"(out),"m"(cross),"m"(dir),"m"(side));
 }
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(obj->xB0):"r"(out),"r"(origin));
}
