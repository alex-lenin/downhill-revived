/* UNIT 0x001AEC60 0x001AEED0 */
#include "rider.h"
#define SUB(dst,a,b) __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define NORMAL(inv,len,x,y,z) __asm__ volatile ("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv),"=&f"(len) : "f"(x),"f"(y),"f"(z),"f"(z) : "$2")
typedef Vector4 Vec;
typedef struct { char pad[0x10]; Matrix4 matrix; } Bone;
typedef struct { char pad[0xa4]; Bone *xA4,*xA8; } Obj;
extern Vec D_007685A0[];
static inline void Aim(Bone **bone,Vec *target,Vector4 *src,SceneNode *bike,Vec *diff) {
 float zero,neg,y,z,len,inv,ny,nz;
 SUB(*diff,src,&bike->position);
 func_00221888(&bike->matrix,diff,diff);
 SUB(*diff,target,diff);
 diff->x=0;
 y=diff->y;zero=diff->x;z=diff->z;
 NORMAL(inv,len,zero,y,z);
 ny=y*inv;nz=z*inv;
 diff->z=nz;neg=-nz;diff->y=ny;
 func_00221F20(&(*bone)->matrix);
 if(ny!=zero && neg!=zero) {(*bone)->matrix.row[1].y=ny;(*bone)->matrix.row[1].z=nz;(*bone)->matrix.row[2].y=neg;(*bone)->matrix.row[2].z=ny;}
}
void func_001AEC60(Obj *obj,RiderComponent *rider) {
 Vec diff,target; SceneNode *bike;
 if(obj->xA4 && obj->xA8) {
 bike=rider->rider->bodyNode;
 SUB(target,&D_007685A0[func_001B1C08(rider)],&bike->position);
 func_00221888(&bike->matrix,&target,&target);
 target.y+=0.5f;
 Aim(&obj->xA4,&target,&rider->x1E70,bike,&diff);
 Aim(&obj->xA8,&target,&rider->x1D60,bike,&diff);
 }
}
