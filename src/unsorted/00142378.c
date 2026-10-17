/* UNIT 0x00142378 0x00142508 */
typedef struct Vec { float v[4]; } Vec __attribute__((aligned(16)));
typedef struct Obj { Vec x0; char pad10[0x40]; int x50,x54; char pad58[4]; int x5C; char pad60[0x28]; int x88; char pad8C[0x64]; char *xF0; char padF4[0xC]; int x100; float x104; int x108; float x10C; int x110,x114,x118; char pad11C[0x50]; int x16C; } Obj;
extern char D_003C2230[];
extern void func_00222C38(Vec *,float),func_001820C8(void *,int,float *,float *,float *);
extern void func_001425B0(Obj *,int,int),func_002226C8(void *,Vec *),func_001425E0(Obj *);
void func_00142378(Obj *obj,Vec *input) {
 Vec first, dir;
 Vec *ptr;
 float length, inv;
 if(obj->xF0) {
  func_00222C38(&first,1.0f);
  if(!obj->x16C) { input=&dir; func_001820C8(D_003C2230,obj->x88,&dir.v[0],&dir.v[1],&dir.v[2]); __asm__ volatile("lqc2 $vf11,0(%0)" : : "r"(input)); }
  else { __asm__ volatile("lqc2 $vf11,0(%0)" : : "r"(input)); }
  ptr=&dir;
  __asm__ volatile("qmfc2.ni $2,$vf12\n\tmfc1 $2,%0\n\tqmtc2.ni $2,$vf12" : : "f"(1.0f) : "$2");
  __asm__ volatile("lqc2 $vf13,%0" : : "m"(first));
  __asm__ volatile("qmfc2.ni $2,$vf14\n\tmfc1 $2,%0\n\tqmtc2.ni $2,$vf14" : : "f"(0.24444444f) : "$2");
  __asm__ volatile("vmulax.xyz ACC,$vf11,$vf12x\n\tvmaddx.xyz $vf11,$vf13,$vf14x\n\tsqc2 $vf11,%0" : "=m"(obj->x0));
  obj->x50=1;obj->x54=0;
  func_001425B0(obj,0,30);
  obj->x5C=6;obj->x0.v[2]+=0.29333332f;
  __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(length) : "f"(obj->x0.v[0]),"f"(obj->x0.v[1]),"f"(obj->x0.v[2]),"f"(obj->x0.v[2]) : "$2");
  dir.v[0]=obj->x0.v[0]*inv;dir.v[1]=obj->x0.v[1]*inv;dir.v[2]=obj->x0.v[2]*inv;
  func_002226C8(obj->xF0+0x40,ptr);
  __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(obj->x0) : "r"(ptr),"f"(0.6111111f) : "$2");
  obj->x100=0;obj->x108=0;obj->x110=0;obj->x114=0;obj->x118=0;obj->x104=0.6111111f;obj->x10C=0.6111111f;obj->x0.v[3]=0.6111111f;
  func_001425E0(obj);
 }
}
