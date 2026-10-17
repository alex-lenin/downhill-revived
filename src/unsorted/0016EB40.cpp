/* UNIT 0x0016EB40 0x0016ED38 .rodata=0x28B010:0x28B040 */
typedef struct { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Transform { unsigned long flags; char pad8[8]; Vec pos; };
class Part { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual Transform *GetTransform(); };
struct Body { char pad0[0x10]; Vec pos; };
struct Sub { char pad0[8]; int state; char padC[0x124]; Part *part; char pad134[0x9C]; Vec pos; char pad1E0[0x50]; };
struct Inner { char pad0[0x1F0]; Body * volatile body; };
struct Obj { int type; char pad4[0xC]; Inner inner; char pad204[0x4C]; Sub first; Sub second; char pad6B0[0xE]; char x6BE,x6BF; };
extern "C" { extern char D_00412510[]; extern Vec *func_00164170(void *,void *,Vec *); extern void func_00126B20(Sub *,int); }
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void CopyXYZ(Vec *dst,Vec *src) { float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w; }
extern "C" void func_0016EB40(Obj *obj) {
 Vec a,b; Vec *p; Sub *sub; Inner *inner;
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(a));
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(b));
 obj->x6BE=0; obj->x6BF=0;
 switch(obj->type) {
 case 3: case 8: case 12:
 inner=(Inner *)((unsigned int)obj+0x10);
 COPY_VECTOR(a,&inner->body->pos); COPY_VECTOR(b,&inner->body->pos); break;
 case 1: case 5: case 10:
 p=func_00164170(D_00412510,obj->first.part ? &obj->first.part->GetTransform()->pos : 0,0);
 if(p) { COPY_VECTOR(a,p); }
 p=func_00164170(D_00412510,obj->second.part ? &obj->second.part->GetTransform()->pos : 0,p);
 if(p) { COPY_VECTOR(b,p); }
 break;
 }
 sub=&obj->first;
 if(sub->state!=3) { sub->part->GetTransform()->flags |= (unsigned long)0x8000 << 42; CopyXYZ(&obj->first.pos,&a); func_00126B20(sub,3); }
 sub=&obj->second;
 if(sub->state!=3) { sub->part->GetTransform()->flags |= (unsigned long)0x8000 << 42; CopyXYZ(&obj->second.pos,&b); func_00126B20(sub,3); }
}
