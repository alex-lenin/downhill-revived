/* UNIT 0x00136790 0x00136AA8 .sdata=0x29C318:0x29C364 */
typedef float Vec4[4] __attribute__((aligned(16)));
typedef struct World { char pad0[0x3f58]; float x3f58; char pad3f5c[0x4098-0x3f5c]; unsigned long x4098; char pad40a0[0x4ecc-0x40a0]; void *x4ecc; char pad4ed0[0x791c-0x4ed0]; float x791c; char pad7920[0x7a69-0x7920]; unsigned char x7a69; char pad7a6a[14]; float x7a78; char pad7a7c[0x7baa-0x7a7c]; unsigned char x7baa; } World;
typedef struct Obj { char pad0[0x38]; float *x38; char pad3c[0xc0-0x3c]; Vec4 xc0; char padd0[0x150-0xd0]; float x150; int x154; World *x158; int x15c; int x160; char pad164[0x228-0x164]; float x228; char pad22c[0x396-0x22c]; struct { unsigned long value:16; } __attribute__((packed)) x396; char pad398[0x3b0-0x398]; Vec4 x3b0; } Obj;
float D_0029C318=20.0f;
float D_0029C31C=0.1f;
float D_0029C320=0.4f;
float D_0029C324=0.25f;
float data_29C328=0.0f;
int data_29C32C=10;
float D_0029C330=5.0f;
float data_29C334=1.024999976f;
float data_29C338=0.6999999881f;
int D_0029C33C=0;
int data_29C340=0, data_29C344=0, data_29C348=0, data_29C34C=0, data_29C350=0, data_29C354=0, data_29C358=0, data_29C35C=0, data_29C360=0;
extern float D_007684A8[],D_007684A4[];
extern float func_0012E070(World *),func_00238F38(void),func_0016AF28(void *),func_0012C990(World *);
extern void func_00231C20(Obj *,float *),func_00232AD0(Obj *,float *);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static __inline__ float length2(float *a) { float r; __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(r) : "f"(a[0]),"f"(a[1]),"f"(a[2]));return r; }
void func_00136790(Obj *o) {
 Vec4 axis,force,velocity;
 float strength,len,inv,x,y,z,squared,root;
 World *w,*w2;
 float *basis,*forward;
 int type;
 w=o->x158;
 if(((unsigned char)(w->x4098>>2))&1) return;
 if(w->x7a78==0.0f && w->x7a69==0 && w->x4ecc!=(char *)w+0x5e70 && func_0012E070(w)==0.0f) return;
 strength=o->x150*(func_00238F38()*0.5f);
 if(func_0012E070(o->x158)==0.0f) {
  w2=o->x158;
  if(w2->x7a78!=0.0f) strength*=w2->x7a78*D_007684A8[0];
  if(w2->x7a69) { strength*=1.5f; strength*=w2->x3f58; strength*=D_007684A4[0]; }
 } else strength*=func_0016AF28(&o->x160);
 type=((unsigned short)o->x396.value)&31;
 if(type==22) strength*=1.15f;
 else if(type==11) strength*=1.25f;
 strength*=o->x158->x791c;
 basis=o->x3b0; forward=o->x38;
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(axis) : "r"(basis),"r"(forward) : "memory");
 x=axis[0]; y=axis[1]; z=axis[2];
 /* Sony libvu0 length and reciprocal wrapper. */
 __asm__ volatile("mula.s %2,%2
	madda.s %3,%3
	madd.s %1,%4,%4
	sqrt.s %1,%1
	lui $2,0x3f80
	mtc1 $2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv),"=&f"(root) : "f"(x),"f"(y),"f"(z),"f"(z) : "$2");
 axis[0]=x*inv; axis[1]=y*inv; axis[2]=z*inv;
 if(root<0.0001f) COPY_VECTOR(axis,forward+4);
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(force) : "r"(axis),"f"(strength) : "$2","memory");
 func_00231C20(o,force);
 w=o->x158;
 if(w->x7a78!=0.0f || w->x7a69!=0) {
  COPY_VECTOR(velocity,o->xc0);
  squared=length2(velocity);
  strength=D_0029C318*1.4666667f;
  if(w->x7baa) strength*=func_0012C990(w);
  if(squared<strength*strength && ((o->x160!=2 && o->x228>0.7f) || D_0029C33C)) {
   len=strength*D_0029C31C;
   __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(velocity) : "r"(velocity),"r"(axis),"f"(len) : "$2","memory");
   func_00232AD0(o,velocity);
  }
 }
}
