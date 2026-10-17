/* UNIT 0x001703B0 0x00170740 .sdata=0x29C8B4:0x29C8B8 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Cam { char pad0[0x10]; Vec x10; Vec x20; } Cam;
typedef struct Inner {
 char pad0[0x40C0]; char x40C0[0xC0]; Vec x4180; char pad4190[0xF0]; Vec x4280;
 char pad4290[0xC3C]; void *x4ECC; char pad4ED0[0x1410]; char x62E0[0x18C4]; Cam *x7BA4;
} Inner;
typedef struct Obj { Inner *x0; char pad4[0xC]; Vec x10; float x20; char pad24[0xC]; Vec x30; Vec x40; int x50; } Obj;
#define COPY_VECTOR(dst, src) \
 __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
extern void func_00135708(void *,Vec *);
float D_0029C8B4 = 0.0f;
static inline void Sub(Vec *dst,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b)); }
static inline void Add(Vec *dst,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b)); }
static inline void Scale(Vec *dst,Vec *src,float scale) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(src),"f"(scale) : "$2"); }
static inline void Cross(Vec *dst,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*dst) : "r"(a),"r"(b)); }
static inline float Dot(Vec *a,Vec *b) { float d; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return d; }
static inline void Normalize(Vec *dst,Vec *src) {
 float x=src->x,y=src->y,z=src->z,len,inv; int bits;
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 dst->x=x*inv; dst->y=y*inv; dst->z=z*inv;
}
static inline void SetVec(Vec *v,float x,float y,float z,float w) { v->x=x; v->y=y; v->z=z; v->w=w; }
static inline float LengthSq(Vec *a) { float d; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(d) : "f"(a->x),"f"(a->y),"f"(a->z)); return d; }
int func_001703B0(Obj *obj) {
 Vec diff,closest,pos,base,dir,proj,toward,fwd,ref,side,up;
 Inner *inner=obj->x0;
 Cam *cam=inner->x7BA4;
 int result=0;
 float t,facing;
 if(inner->x4ECC==inner->x62E0) return 0;
 if(obj->x50>0) obj->x50--;
 func_00135708(inner->x40C0,&pos);
 COPY_VECTOR(base,&cam->x20);
 base.z+=D_0029C8B4;
 Sub(&diff,&pos,&base);
 Scale(&dir,&cam->x10,1000.0f);
 t=Dot(&dir,&diff)/LengthSq(&dir);
 Scale(&proj,&dir,t);
 Add(&closest,&base,&proj);
 Sub(&toward,&closest,&pos);
 Normalize(&toward,&toward);
 Normalize(&fwd,&inner->x4180);
 facing=Dot(&toward,&fwd);
 if(closest.z<pos.z+2.1f && facing>0.0f) {
  Sub(&obj->x10,&closest,&pos);
  obj->x20=0.0f;
  result=1;
  COPY_VECTOR(ref,&inner->x4280);
  SetVec(&up,0.0f,0.0f,1.0f,0.0f);
  Cross(&side,&cam->x10,&up);
  Cross(&obj->x40,&side,&cam->x10);
  if(Dot(&ref,&cam->x10)>0.0f) {
   Scale(&side,&side,-1.0f);
   Add(&obj->x30,&side,&cam->x10);
   Normalize(&obj->x30,&obj->x30);
  } else {
   Add(&obj->x30,&side,&cam->x10);
   Normalize(&obj->x30,&obj->x30);
  }
 }
 return result;
}
