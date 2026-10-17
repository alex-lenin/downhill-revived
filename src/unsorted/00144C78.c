/* UNIT 0x00144C78 0x00144E50 */
#include "object_registry.h"
#include "scene.h"
typedef struct Header { unsigned int low:7; unsigned int index:11; unsigned int high:14; } Header;
typedef SceneNode Obj;
typedef struct Owner { char pad0[8]; Obj *x8; void *xC; char pad10[0x180]; int x190; int x194; float x198,x19C,x1A0; char pad1A4[0x10]; int x1B4; } Owner;
typedef struct Input { char pad0[0x1548]; float x1548,x154C; } Input;

extern void func_00203590(void *,unsigned int,int,int);
extern void func_001DCDC8(void *),func_001DDB58(void *,void *),func_00144E90(Owner *,int),func_00145998(Owner *,int),func_00211280(Obj *),func_001450A0(Owner *,int);
extern char D_003705C0[];
extern int D_004B2840[];
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00144C78(Owner *self,Input *input,Obj *obj,int unused,int mode) {
 float v[4]; float scale;
 if(mode) {
 self->x8=obj;
 if(obj) {
 obj->flags |= (SceneNodeFlags)0x8000 << 42;
 if(self->x8->xB4==0) {
 self->x1B4=(*self->x8->resourceWords>>7)&0x7FF;
 self->x8->xB4=RegisterObjectRetail((void *)(self), (unsigned int)(self->x8->resourceWords));
 ((Header *)self->x8->resourceWords)->index=self->x8->xB4;
 }
 func_00203590((char *)self+0x10,*self->x8->resourceWords>>18,self->x1B4,1);
 func_00203590((char *)self+0x20,*self->x8->resourceWords>>18,self->x1B4,2);
 func_00203590((char *)self+0x30,*self->x8->resourceWords>>18,self->x1B4,3);
 self->x8->flags |= (SceneNodeFlags)0x8000 << 41;
 v[0]=1.5f; v[1]=1.5f; v[2]=1.5f;
 COPY_VECTOR(self->x8->x30,v);
 }
 self->xC=input; self->x190=0;
 self->x198=input->x1548; self->x19C=input->x154C;
 if(D_004B2840[0]==2)self->x1A0=1.0f; else self->x1A0=2.0f;
 func_001DCDC8((char *)self+0x40); func_001DDB58((char *)self+0xB0,D_003705C0);
 func_00144E90(self,mode); func_00145998(self,1); func_00211280(self->x8); func_001450A0(self,0);
 }
}
