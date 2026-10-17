/* UNIT 0x0012D660 0x0012D838 */
#include "motion_interpolation.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Transform { char pad[0x20]; Vec x20; };
struct Inner { char pad[0x16D4]; struct Transform *x16D4; };
struct Flags { unsigned long x0:2; unsigned long x2:8; };
struct Obj { char pad0[0x1E40]; struct Inner x1E40; char pad3518[0x4098-0x3518]; unsigned long x4098; char pad40A0[0x7710-0x40A0]; Vec x7710; char pad7720[0x792C-0x7720]; struct Transform *x792C; char pad7930[0x796C-0x7930]; int x796C; char pad7970[0x7988-0x7970]; int x7988; char pad798C[0x7A5C-0x798C]; unsigned int x7A5C; char pad7A60[0x20]; Vec x7A80,x7A90; char pad7AA0[0xF0]; unsigned char x7B90; char pad7B91[3]; struct Obj *x7B94; int x7B98; };
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline void Scale(Vec *out,Vec *v,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"f"(scale) : "$2");
}
static inline void AddScale(Vec *out,Vec *a,Vec *b,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b),"f"(scale) : "$2");
}
static inline void Copy3(Vec *out,Vec *v) { float x=v->x,y=v->y,z=v->z; out->x=x; out->y=y; out->z=z; }
static inline unsigned char Not3(struct Obj *obj) { return obj->x7A5C!=3; }
#if defined(__mips__)
void func_0012D660(struct Obj *obj,int id,float blend)
#else
void SharedCall0012D660(struct Shared0012D660_struct_Obj * obj_view, int id, float blend)
#endif
{
#if !defined(__mips__)
    struct Obj * obj = (struct Obj *)obj_view;
#endif

 Vec v; Vec *smooth; float intensity,scale=0.026041666f; struct Obj *other; int count; struct Transform *dest; struct Inner *inner;
 COPY_VECTOR(v,&obj->x7710);
 smooth=&obj->x7A80;
 v.w=smooth->w;
 Scale(&v,&v,blend);
 AddScale(&v,&v,smooth,1.0f-blend);
 Copy3(smooth,&v);
 Scale(&obj->x7A90,&v,0.021701388f);
 intensity=(v.x*0.3f+v.y*0.6f+v.z*0.1f)*scale;
 if(obj->x796C!=0x200 && obj->x7B90) {
  other=obj->x7B94;
  if(other && Not3(other) && (id==-1 || id==other->x7988) && !((unsigned char)(obj->x4098>>2)&1)) {
   count=obj->x7B98+1; obj->x7B98=count;
   if(count<6) {v.x*=4.0f; v.y=0.0f; v.z=0.0f;}
   else if(count>=12) obj->x7B98=0;
  }
 }
 dest=obj->x792C; inner=&obj->x1E40;
 Copy3(&dest->x20,&v); Copy3(&inner->x16D4->x20,&v);
 obj->x792C->x20.w=intensity; inner->x16D4->x20.w=intensity;
}
