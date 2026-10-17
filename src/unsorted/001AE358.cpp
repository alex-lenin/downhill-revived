/* UNIT 0x001AE358 0x001AE630 */
#include "camera.h"
#include "rider.h"
typedef Vector4 Vec;
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define VADD(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%3)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(&(dst)),"r"(a),"r"(b))
#define VSUB(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define VSCALE(dst,a,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(f) : "$2")
#define VCROSS(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define VCROSS_P(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%2)
	lqc2 $vf12,0(%3)
	vopmula.xyz ACC,$vf11,$vf12
	vopmsub.xyz $vf11,$vf12,$vf11
	sqc2 $vf11,%0" : "=m"(dst) : "r"(&(dst)),"r"(a),"r"(b))
#define VDOT(result,a,b) __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"((a).x),"f"((b).x),"f"((a).y),"f"((b).y),"f"((a).z),"f"((b).z))
/* Sony scalar vector normalization uses the EE accumulator. */
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z;
 __asm__ volatile ("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z) : "$2");
 v->x=x*inv;v->y=y*inv;v->z=z*inv;
}
static inline void NormalizeCross(Vec *v) {
 float length,inv;
 __asm__ volatile ("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%5
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv), "=&f"(length) : "f"(v->x), "f"(v->y), "f"(v->z), "f"(v->z) : "$2");
 v->x*=inv;v->y*=inv;v->z*=inv;
}
#define VNORMALIZE(v) Normalize(&(v))
#define VTRANS(dst,a,m) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(m))
#define VMADD(dst,a,b,f) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b),"f"(f) : "$2")
typedef CameraAngles Output;
typedef RiderComponent Player;
typedef RiderCamera Input;
extern "C" Vec D_00769B30[];

extern "C" float func_001385A8(void *);
extern "C" void func_00221AB8(void *,Vec *,Vec *);

extern "C" float func_00236088(float,float,float,float);
extern "C" void func_001AE358(Output *out,Input *in) {
 Vec a,b,c,d,e; char unused[16]; Vec matrix[4]; Vec angles; Player *player; Vec *basis; float blend,x,y,z;
 player=in->xA10; basis=&D_00769B30[func_001B1C08(player)*4];
 blend=func_001385A8((char *)player->rider+0x40C0);
 COPY_VECTOR(b,&player->rider->physics.velocity); b.z*=0.5f; VNORMALIZE(b);
 VSCALE(b,&b,blend); VSCALE(a,basis,1.0f-blend); VADD(c,&b,&a); VNORMALIZE(c);
 COPY_VECTOR(e,basis+2); VCROSS_P(d,&c,&e); NormalizeCross(&d); VCROSS(e,&d,&c);
 func_00221AB8(matrix,&c,basis); func_002224E0((const Matrix4 *)matrix,(Vector3 *)&angles);
 angles.y=(angles.y >? -2.269f) <? 2.269f;
 angles.x=(angles.x >? -0.262f) <? 0.262f;
 angles.z=(angles.z >? -1.134f) <? 1.483f;
 out->xA0=func_00236088(out->xA0,angles.x,0.1f,0.05f);
 out->xA4=func_00236088(out->xA4,angles.y,0.1f,0.05f);
 out->xA8=func_00236088(out->xA8,angles.z,0.1f,0.05f);
}
