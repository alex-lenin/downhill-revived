/* UNIT 0x0011B268 0x0011B508 */
#include "rider_queries.h"
#if defined(__mips__)
#define ReturnShared0011B268(value) (value)
#else
#define ReturnShared0011B268(value) ((struct Rider *)(value))
#endif
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VADD(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define VSUB(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define VSCALE(dst,a,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(f) : "$2")
#define VCROSS(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define VDOT(result,a,b) __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"((a).x),"f"((b).x),"f"((a).y),"f"((b).y),"f"((a).z),"f"((b).z))
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z; union { int bits; float f; } one;
 __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=&f"(length) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile ("sqrt.s %0,%1" : "=f"(length) : "f"(length));
 one.bits=0x3f800000; inv=one.f/length;v->x=x*inv;v->y=y*inv;v->z=z*inv;
}
#define VNORMALIZE(v) Normalize(&(v))
#define VTRANS(dst,a,m) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(m))
#define VMADD(dst,a,b,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b),"f"(f) : "$2")
struct Body { char pad0[16]; Vec x10; char pad20[48]; Vec x50; };
struct Rider { char pad0[0x4180]; Vec x4180; char pad4190[0x379C]; struct Body *x792C; char pad7930[0x2A0]; };
struct World { struct Rider riders[10]; char pad4D620[0x2B88]; int count; };
extern float D_0029C154[];
extern float func_001385A8(void *);
static inline float Dot(Vec *a,Vec *b) {
 float ax=a->x,bx=b->x,ay=a->y,by=b->y,az=a->z,bz=b->z,result;
 __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(result) : "f"(ax),"f"(bx),"f"(ay),"f"(by),"f"(az),"f"(bz));
 return result;
}
static inline float SquaredLength(Vec *v) {
 float result;
 __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=f"(result) : "f"(v->x),"f"(v->y),"f"(v->z));
 return result;
}
#if defined(__mips__)
struct Rider *func_0011B268(struct World *world,Vec *pos,float *dist,float maxZ,float minZ,Vec *outPos)
#else
struct Rider * SharedCall0011B268(struct Shared0011B268_struct_World * world_view, struct Vector4 * pos_view, float * dist, float maxZ, float minZ, struct Vector4 * outPos_view)
#endif
{
#if !defined(__mips__)
    struct World * world = (struct World *)world_view;
    Vec * pos = (Vec *)pos_view;
    Vec * outPos = (Vec *)outPos_view;
#endif

 Vec d,p,n; struct Rider *best,*rider; float projection,length,t,distance; int count,i; float negative;
 count=world->count;best=0;
 if(dist) *dist=D_0029C154[0];
 if(count>0) {negative=-1.0f;
 for(i=0;i<count;i++) {
 rider=&world->riders[i]; VSUB(d,&rider->x792C->x10,pos);
 if(d.z>minZ && d.z<maxZ) {
 if(func_001385A8((char *)rider+0x40C0)>0.05f) {COPY_VECTOR(n,&rider->x4180);} else {COPY_VECTOR(n,&rider->x792C->x50);}
 COPY_VECTOR(p,&rider->x792C->x10);VSCALE(d,&d,negative);projection=Dot(&n,&d);
 if(projection>0.0f) {
 length=Dot(&n,&n); t=projection/length;
 if(t>0.4f) {VMADD(p,&p,&n,0.4f);} else {VMADD(p,&p,&n,t);}
 VSUB(d,&p,pos);distance=SquaredLength(&d);
 } else {distance=SquaredLength(&d);}
 if(distance<*dist) {best=rider;if(dist) *dist=distance;if(outPos) {COPY_VECTOR(*outPos,&p);}}
 }
 }
 }
 return ReturnShared0011B268(best);
}
