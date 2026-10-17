/* UNIT 0x00122830 0x00122AA0 */
struct Vec { float x,y,z,w; } __attribute__((aligned(16)));
struct Transform { unsigned long flags; char pad8[8]; Vec x10; char pad20[0x20]; char x40[0x60]; char xA0[0x40]; };
class Part { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual Transform *GetTransform(); };
struct Body { char pad0[16]; Vec x10; };
struct Rider { char pad0[0x792C]; Body *x792C; };
struct State { char pad0[0x40]; void *x40; };
struct Obj { char pad0[0x14]; int x14; int x18; int x1C; int x20; char pad24[0xDC]; Vec x100,x110; char pad120[16]; Part *x130; char pad134[8]; Rider *x13C; char pad140[0x2C]; char x16C[0x14]; char x180[0x40]; void *x1C0; char pad1C4[0x1C]; Vec x1E0,x1F0,x200; };
struct Input { Vec pos; char pad10[6]; short x16; };
extern "C" { void func_00123DB0(Obj *,void *); void func_00123F60(Obj *,int); void func_00211280(Transform *); void func_00221F50(void *,void *); void func_002225A8(void *,void *); void func_00261160(void *); }
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void CopyXYZ(Vec *dst, Vec *src) { float w=dst->w; COPY_VECTOR(*dst,src); dst->w=w; }
static inline void Normalize(Vec *v) {
 float len,inv;
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len) : "f"(v->x),"f"(v->y),"f"(v->z),"f"(v->z) : "$2");
 v->x*=inv; v->y*=inv; v->z*=inv;
}
extern "C" void func_00122830(Obj *obj, Input *input) {
 Vec dir,pos,zero;
 void *matrix; State *state;
 if (obj->x130->GetTransform()!=0) {
 obj->x14=0; obj->x1C=0; obj->x20=0; state=(State *)obj->x180; state->x40=obj->x16C;
 func_00261160(state);
 COPY_VECTOR(pos,&input->pos);
 if (input->x16>0) { CopyXYZ(&dir,(Vec *)(obj->x180+0x10)); }
 else {
 Vec *from=&obj->x13C->x792C->x10;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dir) : "r"(from),"r"(&pos));
 Normalize(&dir);
 }
 COPY_VECTOR(obj->x130->GetTransform()->x10,&pos);
 func_002225A8(obj->x130->GetTransform()->xA0,&dir);
 matrix=obj->x130->GetTransform()->x40;
 func_00221F50(matrix,obj->x130->GetTransform()->xA0);
 func_00123F60(obj,1);
 COPY_VECTOR(obj->x100,&obj->x110);
 CopyXYZ(&obj->x1E0,&pos);
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(zero));
 CopyXYZ(&obj->x1F0,&zero);
 CopyXYZ(&obj->x200,&dir);
 func_00211280(obj->x130->GetTransform());
 obj->x130->GetTransform()->flags &= 0xFDFFFFFFFFFFFFFFUL;
 func_00123DB0(obj,obj->x130->GetTransform()->x40);
 }
}
