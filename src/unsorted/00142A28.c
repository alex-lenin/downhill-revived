/* UNIT 0x00142A28 0x00142D50 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union { u128 q; float f[4]; } Vec;
typedef struct { float x0[4]; char pad10[0x4C]; int x5C; char pad60[0x40]; float xA0[4]; char padB0[0x10]; float xC0[4]; char padD0[0x1C]; float xEC; char *xF0; char padF4[12]; float x100[4],x110[4]; char pad120[32]; float x140[4],x150[4]; int x160,x164,x168,x16C; } Obj;
extern float D_0029DD40,D_0029DD44,D_0029DD48;
typedef struct { float x0,x4,x8,xC; } Table;
extern Table D_0026E480[];
extern void func_00221888(void *,void *,void *);
static inline void Scale(float *out,float *in,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" : : "r"(out),"r"(in),"f"(scale):"$2","memory");
}
static inline void AddScale(float *out,float *a,float *b,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvadda.xyz ACC,$vf11,$vf0\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,0(%0)" : : "r"(out),"r"(a),"r"(b),"f"(scale):"$2","memory");
}
static inline void Add(float *out,float *a,float *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(out),"r"(a),"r"(b):"memory");
}
static inline void Transform(float *out,char *matrix,float *in) {
 __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,16(%1)\n\tlqc2 $vf14,32(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)" : : "r"(out),"r"(matrix),"r"(in):"memory");
}
void func_00142A28(Obj *obj) {
 Vec force,transformed;
 float delta,y,z,x,target,limit,height,speed; Table *row;
 force.q=0; force.f[2]=-0.008f;
 Scale(obj->x140,obj->x110,0.02f);
 limit=obj->xC0[3]; height=obj->xEC;
 if(height<limit) {
  delta=(height-limit)*0.003f;
  if(-delta>0.08f) delta=-0.08f;
  AddScale(obj->x140,obj->x140,obj->xC0,delta);
  AddScale(obj->x140,obj->x140,obj->xA0,0.02f);
 }
 func_00221888(obj->xF0+64,obj,obj->x100);
 func_00221888(obj->xF0+64,&force,&transformed);
 func_00221888(obj->xF0+64,obj->x140,obj->x140);
 x=obj->x100[0]; y=obj->x100[1]; z=obj->x100[2];
 obj->x150[1]=-D_0029DD40*y*y*y;
 obj->x150[0]=x*(x*-0.050000012f)*x;
 obj->x150[2]=z*(z*-0.5f)*z;
 __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0" : "=m"(obj->x150) : "r"(obj->x150),"r"(transformed.f):"memory");
 obj->x164=0;
 if(obj->x16C) {
  target=D_0029DD44*0.024444444f; speed=obj->x100[1];
  if(speed+0.0009765625f<target || speed<0.2444f) {
   if(((Table *)(obj->x5C*16+(char *)D_0026E480))->x4<=-obj->x150[1]) obj->x164=1;
  } else if(target<speed-0.0009765625f) {
   if(-obj->x150[1]<=((Table *)(obj->x5C*16+(char *)D_0026E480))->x4) obj->x164=-1;
  }
 }
 row=&D_0026E480[obj->x5C];
 obj->x100[0]+=obj->x150[0]+obj->x140[0];
 obj->x100[1]+=obj->x150[1]+row->x4;
 obj->x100[2]+=obj->x150[2]+obj->x140[2]+row->x8*D_0029DD48;
 Transform(obj->x0,obj->xF0+64,obj->x100);
}
