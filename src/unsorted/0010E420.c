/* UNIT 0x0010E420 0x0010E6C0 */
#include "vector.h"
typedef Vector4 Vec;
typedef struct Frame { char pad[0x10]; Vec x10; char pad20[0x20]; Vec matrix[4]; } Frame;
typedef struct World { char pad[0x792c]; Frame *x792C; } World;
typedef struct Obj { World *x0,*x4; char pad8[0x28c]; float x294; char pad298[0x1a8]; unsigned long x440; } Obj;
extern float func_001385A8(void *);
extern int func_0010E918(Obj *,void *,void *),func_0010E9B0(Obj *,int,World *),func_00222B08(int,int);
extern void func_001C7788(void *),func_001C7848(void *),func_001C7A10(void *,void *),func_0010E860(Obj *),func_0010F068(Obj *,int);
void func_0010E420(Obj *obj,int arg1) {
 Vec zero,pos,diff; Vec *position; int result=0; World *other,*current,*sound; Frame *frame; float dot,speed; Vec *axis,*otherpos;
 if(obj->x440 & 0x10000000000UL) {
 other=obj->x4;
 if(other && func_001385A8((char *)other+0x40c0)>0.2f && ((obj->x440 & 0x4000000000000UL) || func_0010E918(obj,&obj->x4->x792C->x10,&obj->x0->x792C->x10))) {
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(zero) : : "memory");
 other=obj->x4;
 __asm__ volatile("lqc2 $vf11,%1\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(pos) : "m"(zero),"r"(other->x792C->matrix) : "memory");
 frame=other->x792C; frame=(Frame *)((char *)frame+0x10); position=&pos;
 __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "+m"(pos) : "r"(frame),"r"(position) : "memory");
 current=obj->x0;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(&current->x792C->x10),"r"(position) : "memory");
 frame=other->x792C; axis=(Vec *)((char *)frame+0x50);
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dot) : "f"(axis->x),"f"(diff.x),"f"(axis->y),"f"(diff.y),"f"(axis->z),"f"(diff.z));
 if(dot<0.0f) {
 result=func_0010E9B0(obj,arg1,current);
 if(obj->x294<10.0f) {
 if(!func_00222B08(0,1)) sound=obj->x0; else {sound=0; if(obj->x440 & 0x10000000000UL) sound=obj->x4;}
 func_001C7788((char *)sound+0x7720);
 }
 } else {
 speed=obj->x294;
 if(speed<15.0f) {
 if(!func_00222B08(0,1)) sound=obj->x0; else {sound=0; if(obj->x440 & 0x10000000000UL) sound=obj->x4;}
 func_001C7848((char *)sound+0x7720);
 } else if(speed<20.0f) func_001C7A10((char *)current+0x7720,&pos);
 }
 }
 }
 if(!result) {
 if(obj->x440 & 0x20000000000UL) {func_0010E860(obj); obj->x440 &= ~0x20000000000UL;}
 func_0010F068(obj,arg1);
 }
}
