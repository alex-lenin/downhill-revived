/* UNIT 0x00136038 0x00136790 .sdata=0x29C370:0x29C390 */
#include "impulse.h"
typedef float Vec4[4] __attribute__((aligned(16)));
typedef struct World {
 char pad0[0x3f58]; float x3f58;
 char pad3f5c[0x4098-0x3f5c]; unsigned long x4098;
 char pad40a0[0x4ecc-0x40a0]; void *x4ecc;
 char pad4ed0[0x6d28-0x4ed0]; int x6d28;
 char pad6d2c[0x7914-0x6d2c]; float x7914;
 char pad7918[8]; float x7920;
 char pad7924[0x7964-0x7924]; short x7964;
 char pad7966[6]; int x796c;
 char pad7970[0x7a69-0x7970]; unsigned char x7a69;
 char pad7a6a[0x7af0-0x7a6a]; float x7af0;
 char pad7af4[0x7bb1-0x7af4]; unsigned char x7bb1;
} World;
typedef struct Obj {
 char pad0[0x38]; float *x38;
 char pad3c[0x70-0x3c]; Vec4 x70;
 char pad80[0xc0-0x80]; Vec4 xc0;
 char padd0[0x100-0xd0]; Vec4 x100,x110;
 char pad120[0x150-0x120]; float x150,x154; World *x158;
 char pad15c[0x410-0x15c]; float x410,x414,x418,x41c,x420,x424,x428;
 char pad42c[0x488-0x42c]; float x488,x48c,x490; int x494;
 char pad498[0x4a4-0x498]; short x4a4,x4a6,x4a8;
 char pad4aa[0x4b4-0x4aa]; short x4b4; short x4b6; void (*x4b8)(void *);
} Obj;
float D_0029C370=2.0f;
float D_0029C374=2.8f;
float D_0029C378[2]={2.8f,0.0f};
float D_0029C380=1.0f;
float D_0029C384=0.3f;
float D_0029C388=4.0f;
float D_0029C38C=4.0f;
extern float D_0029C2E8,D_0029C2EC,D_0029C314;
extern float D_00768498[],D_0026DE20[];
extern void func_00231C10(Obj *),func_00231C20(Obj *,float *);
extern int func_00138560(Obj *),func_00238F60(void);
extern float func_0012C9B8(World *),func_0012E028(World *),func_001385A8(Obj *);
extern void func_00138658(Obj *),func_00138808(Obj *),func_00136AA8(Obj *),func_00136D68(Obj *),func_00136D30(Obj *),func_00136F80(Obj *);
extern void func_00137950(Obj *),func_00137B70(Obj *),func_00137E68(Obj *),func_00136790(Obj *),func_00137C78(Obj *),func_00137DE8(Obj *),func_00136B40(Obj *),func_00136C20(Obj *);
extern void func_001EA9A0(void *),func_001EAAD0(void *),func_00221C58(float *,float *);
extern float fabsf(float);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(src),"f"(s) : "$2")
static inline float minimum(float a,float b) {
 float r; __asm__ volatile("min.s %0,%1,%2" : "=&f"(r) : "f"(a),"f"(b)); return r;
}
static inline float Dot(float *a,float *b) {
 float r;
 __asm__ volatile("mula.s %1,%2
	madda.s %3,%4
	madd.s %0,%5,%6" : "=f"(r) : "f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2]));
 return r;
}
static inline float normalize(float *dst,float *src) {
 float x=src[0],y=src[1],z=src[2],inv,len;
 /* Sony libvu0 normalization wrapper. */
 __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv),"=&f"(len) : "f"(x),"f"(y),"f"(z) : "$2");
 dst[0]=x*inv;dst[1]=y*inv;dst[2]=z*inv;
 return len;
}
static inline float normalize_dir(float *dst,float *src) {
 float x=src[0],y=src[1],z=src[2],inv,len;
 /* Sony libvu0 normalization wrapper. */
 __asm__ volatile("mula.s %2,%3
	madda.s %4,%5
	madd.s %0,%6,%7
	sqrt.s %0,%0
	lui $2,0x3f80
	mtc1 $2,%1
	nop
	div.s %1,%1,%0" : "=&f"(len),"=&f"(inv) : "f"(x),"f"(x),"f"(y),"f"(y),"f"(z),"f"(z) : "$2");
 dst[0]=x*inv;dst[1]=y*inv;dst[2]=z*inv;
 return len;
}
void func_00136038(Obj *o) {
 Vec4 force;
 float gravity, strength, boost, drag, factor;
 float *velocity,*axis;

 func_00231C10(o);
 gravity=o->x418*(D_00768498[0]*32.2f);
 if(func_00138560(o)) {
  World *w=o->x158;
  if(w->x7bb1 && o->x494 && w->x6d28<=0) gravity*=D_0029C384*D_00768498[1];
  else gravity*=o->x41c*D_00768498[1];
  gravity*=func_0012C9B8(o->x158);
 }
 strength=o->x150*gravity;
 if(((unsigned char)(o->x158->x4098>>2))&1) func_00138808(o);
 else if(func_00138560(o)) {
  func_00138658(o);
  if(o->x488>0.0f && o->x410<0.0f) {
   World *w=o->x158;
   if(w->x7bb1 && o->x494 && w->x6d28<=0) strength+=strength*D_0029C388*o->x488;
   else strength+=strength*D_0029C2E8*o->x488;
  } else if(o->x488<0.0f && o->x414>0.0f) {
   World *w=o->x158;
   if(w->x7bb1 && o->x494 && w->x6d28<=0) strength+=strength*D_0029C38C*o->x488;
   else strength+=strength*D_0029C2EC*o->x488;
  }
 } else func_00138808(o);
 force[0]=0.0f; force[1]=0.0f; force[2]=-strength;
 COPY_VECTOR(o->x100,force);
 func_00231C20(o,force);
 if(!func_00138560(o)) o->x4a8+=func_00238F60();
 boost=func_0012E028(o->x158);
 if(boost>0.0f && (float)o->x4a4<15.0f) {
  boost*=1.0f-(float)o->x4a4*0.06666667f;
  boost=o->x424+boost*(o->x420-o->x424);
  if(func_001385A8(o)<0.5f) {
   float t=0.5f-func_001385A8(o);
   boost-=minimum(boost*0.5f,boost*(t*2.0f));
  }
  if(o->x4a8<30) boost=boost*(float)o->x4a8*0.033333335f;
  factor=o->x150*o->x418*boost*o->x158->x7920;
  force[0]=0.0f; force[1]=0.0f; force[2]=factor;
  func_00231D48((ImpulseBody *)o,(Vector4 *)force); o->x4a8=0;
 }
 func_00136AA8(o); func_00136D68(o); func_00136D30(o); func_00136F80(o);
 {
 World *w;
 drag=D_00768498[2]*(o->x428+(o->x150-100.0f)*0.001f)*1.4666667f;
 w=o->x158;
 if(w->x7a69) {
  float factor,t;
  drag*=w->x3f58*1.15f;
  t=(float)w->x7964*0.016666668f;
  factor=D_0026DE20[(w->x796c>>5)-1];
  drag*=minimum(t,factor)/factor*(D_0029C370-1.0f)+1.0f;
 } else if(w->x4ecc==(char *)w+0x5e70) drag*=1.15f;
 }
 drag*=o->x154; drag*=o->x158->x7af0; drag*=o->x158->x7914;
 o->x490=(drag==0.0f)?0.0f:gravity/(drag*drag);
 if(o->x490>1.0f) o->x490=1.0f;
 if(func_00138560(o)) {
  float t=o->x488;
  float a=0.0f;
  if(t<a && o->x414>a && o->x410>a) { a=fabsf(t); o->x490-=o->x490*a; }
 }
 velocity=o->xc0;
 {
 float root=normalize(o->x110,velocity);
 root=-(root*root)*o->x150*o->x490;
 SCALE(o->x110,o->x110,root);
 func_00231C20(o,o->x110);
 }
 if(func_00138560(o)) {
  float dot,factor;
  func_00221C58(force,velocity);
  axis=o->x38+4;
  dot=Dot(force,axis);
  normalize_dir(force,force);
  { World *w=o->x158;
  if(w->x7bb1 && o->x494 && w->x6d28<=0) factor=dot*D_0029C380*o->x150;
  else factor=dot*D_0029C314*o->x150; }
  SCALE(force,force,factor);
  func_00231C20(o,force);
 }
 if(func_00138560(o)) {
  func_00137950(o);func_00137B70(o);func_00137E68(o);func_001EA9A0((char *)o->x158+0x5ff0);
 } else {
  func_00136790(o);func_00137950(o);func_00137B70(o);
  o->x4b8((char *)o+o->x4b4);
  func_00137C78(o);func_00137DE8(o);func_00136B40(o);func_00136C20(o);
 }
 func_001EAAD0((char *)o->x158+0x5ff0);
 SCALE(o->x70,o->x70,0.9f);
}
