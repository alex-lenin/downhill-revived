/* UNIT 0x001B0410 0x001B0870 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Node { char pad0[0x40]; Vec x40; char pad50[0x10]; Vec x60; } Node;
typedef struct Sub {
 char pad0[0x38]; Vec *x38; char pad3C[0x84]; Vec xC0; char padD0[0x260]; Vec x330; char pad340[0x14]; unsigned char x354; char pad355[0x5B]; Vec x3B0;
 char pad3C0[0x14]; unsigned char x3D4;
} Sub;
typedef struct Third { char pad0[8]; float x8; } Third;
typedef struct Parent {
 char pad0[0x1E40]; char x1E40[0x2280]; Sub x40C0; char pad[0x4ECC-0x40C0-sizeof(Sub)]; void *x4ECC; char pad4ED0[0x1340]; Third x6210;
 char pad621C[0x7C]; char x6298[0x1694]; Node *x792C;
} Parent;
typedef struct Obj { Vec x0; Parent *x10; int x14; float x18; int x1C; int x20; float x24; } Obj;
extern int func_001B1C08(void *);
extern float func_0012DEB0(Parent *);
extern int func_001B08D0(Obj *);
extern int func_00238F60(void);
extern void func_001B0870(Obj *,int);
extern float func_00236088(float,float,float,float);
extern Vec D_007684B0[];
static __inline__ float Dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static __inline__ void Normalize(Vec *v,Vec *src) {
 float x=src->x,y=src->y,z=src->z,len,inv; int bits;
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
#define SCALE(dst,src,s) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(src), "f"(s) : "$2")
#define ADD(dst,a,b) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(dst) : "r"(a),"r"(b))
void func_001B0410(Obj *obj) {
 Vec tmp;
 int index,slot;
 int skip,active;
 Parent *parent;
 Sub *sub;
 Vec *up,*dst;
 float dotA,dotB,steer,slope,d,target,blend,rest;
 Third *third;
 index=func_001B1C08(obj->x10->x1E40);
 parent=obj->x10;
 third=&parent->x6210;
 if (obj->x14==0) {
  dotB=1.0f;
  sub=&parent->x40C0;
  steer=0.0f;
  up=(Vec *)((char *)parent->x792C+0x60);
  dotA=1.0f;
  slope=func_0012DEB0(parent);
  active=0;
  if (parent->x40C0.x354!=2) active=parent->x40C0.x3D4!=2;
  if (active) obj->x20++; else obj->x20=0;
  skip=0;
  if (obj->x20<30 || obj->x10->x4ECC==obj->x10->x6298) skip=1;
  if (!skip) {
   dotA=Dot(up,&sub->x330);
   dotB=Dot(up,&sub->x3B0);
  }
  func_001B08D0(obj);
  if (!(dotA>0.766f) && !(dotB>0.766f) && !(__builtin_fabsf(slope)<0.01f)) {
   Dot(&obj->x10->x792C->x40,&sub->x3B0);
  }
  if (obj->x1C>0) {
   obj->x1C-=func_00238F60();
   if (obj->x1C<=0) obj->x24=0.0f;
  } else obj->x24=0.0f;
  if (!skip) {
   Normalize(&tmp,&sub->xC0);
   d=Dot(&tmp,sub->x38);
   target=-0.75f;
   if (d<0.0f) target=0.75f;
   if (__builtin_fabsf(d)>0.4f) {
    if ((obj->x18>0.0f && d<0.0f) || (obj->x18<0.0f && d>0.0f) || __builtin_fabsf(obj->x18)<0.1f) {
     steer=target;
     obj->x1C=20;
    }
   }
   if (__builtin_fabsf(obj->x24)<__builtin_fabsf(steer)) obj->x24=steer;
   steer=obj->x24;
  }
  obj->x18=func_00236088(obj->x18,steer,0.1f,0.05f);
 } else if (func_001B08D0(obj)) {
  obj->x18=func_00236088(obj->x18,third->x8,0.2f,0.1f);
 } else {
  obj->x18=func_00236088(obj->x18,third->x8,0.06f,0.03f);
 }
 if (obj->x18<0.0f) slot=index+10;
 else if (obj->x18>0.0f) slot=index+7;
 else { func_001B0870(obj,0); return; }
 blend=__builtin_fabsf(obj->x18);
 blend=blend*blend;
 SCALE(tmp,&obj->x0,blend);
 rest=1.0f-blend;
 dst=&D_007684B0[slot];
 SCALE(*dst,dst,rest);
 ADD(*dst,dst,&tmp);
}
