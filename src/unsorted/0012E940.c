/* UNIT 0x0012E940 0x0012F110 */
#include "impulse.h"
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Body { char pad[0x10]; Vec pos; char pad20[0x20]; Vec matrix[3]; };
struct Stats { char pad[0xA14]; float xA14,xA18,xA1C; };
struct Physics { char pad[0x38]; Vec *direction; char pad3C[0x450]; float x48C; };
struct Event { char pad[12]; void *xC; };
struct Game { char pad[0x10C]; int mode; };
extern struct Game D_004B25A0;
extern char D_003B2270[], D_0029C298[];
#define AT(T,p,o) (*(T *)((char *)(p)+(o)))

extern int func_00138560(struct Physics *);

extern void func_00155E78(void *,int);
extern void func_001A77E0(void *,Vec *);
extern void func_0012FE38(void *,struct Event *);
extern void func_001B0AC0(void *,float);
extern int func_00117470(void *);
extern void func_001126A8(void *,int,int),func_00112700(void *,int,int);
extern void func_00112808(void *,int,int),func_00112878(void *,int,int);
extern void func_001C7A60(void *,int,float);
extern void func_001D0A58(void *,int,int);
extern void func_001301E0(void *,int);
extern char *func_00158650(int),*func_00158668(int);
extern void func_00254544(void *,char *),func_002542C4(void *,char *);
extern void func_0017B338(void *,int,int,int,int,void *);
static inline void sub(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void rotate(Vec *out,Vec *v,Vec *m) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"r"(m));
}
static inline void scale(Vec *out,Vec *v,float s) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(v),"f"(s) : "$2");
}
static inline float norm(Vec *out,Vec *v) {
 float len,inv,x=v->x,y=v->y,z=v->z;
 /* SDK vector length and reciprocal sequence. */
 __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %0,%4,%4\n\tsqrt.s %0,%0\n\tlui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%0" : "=&f"(len),"=&f"(inv) : "f"(x),"f"(y),"f"(z) : "$2");
 out->x=x*inv; out->y=y*inv; out->z=z*inv;
 return len;
}
static inline float dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline float hit1(struct Stats *s,float f) { return s->xA18*f; }
static inline float hit0(struct Stats *s,float f) { return s->xA14*f; }
static inline void friction(struct Physics *p) { p->x48C=0.0025f; }
static inline float hit(struct Stats *s,float f) { return s->xA1C*f; }
static inline void animate(char *base,unsigned int index,int value,int effect,char *text) {
 switch(index) { case 0: case 1: case 2: case 3: func_0017B338((index*0x1380+0x150)+base,value,1,1,effect,text); break; }
}
static inline int greater(int limit,int count) { return limit<count; }
static inline void set(Vec *v,float x,float y,float z) { v->x=x; v->y=y; v->z=z; }
void func_0012E940(char *self,int kind,Vec *direction,char *other) {
 Vec delta,local,temp;
 char name[32];
 float force;
 struct Stats *stats;
 struct Physics *physics;
 if((unsigned char)(AT(unsigned long,self,0x4098)>>2)&1) return;
 if(AT(unsigned char,self,0x7BB6)) return;
 AT(int,self,0x79A8)=AT(int,other,0x796C);
 sub(&delta,&AT(struct Body *,self,0x792C)->pos,&AT(struct Body *,other,0x792C)->pos);
 func_00221888((Matrix4 *)(AT(struct Body *,self,0x792C)->matrix), (Vector4 *)(&delta), (Vector4 *)(&local));
 if(local.x>0.0f) { set(&local,1.0f,1.0f,0.3f); }
 else { set(&local,-1.0f,1.0f,0.3f); }
 if(kind!=2 && kind!=7 && kind!=3 && kind!=8) local.y=-local.y;
 rotate(&delta,&local,AT(struct Body *,self,0x792C)->matrix);
 if(kind==1 || kind==6) { force=hit1((struct Stats *)(other+0x3530),3600.0f); friction((struct Physics *)(self+0x40C0)); }
 else if(kind==2 || kind==7) force=hit((struct Stats *)(other+0x3530),4800.0f);
 else if(kind==3 || kind==8) force=hit((struct Stats *)(other+0x3530),4801.0f);
 else if(kind==4 || kind==9) force=hit((struct Stats *)(other+0x3530),4802.0f);
 else { force=hit0((struct Stats *)(other+0x3530),2400.0f); friction((struct Physics *)(self+0x40C0)); }
 stats=(struct Stats *)(other+0x3530);
 physics=(struct Physics *)(self+0x40C0);
 if(((AT(int,other,0x796C)==0xA0 || AT(int,other,0x796C)==0x160) && (kind==0 || kind==5 || kind==1 || kind==6)) || ((AT(int,other,0x796C)==0x200 || AT(int,other,0x796C)==0x180) && (kind==1 || kind==6))) force=stats->xA1C*4800.0f;
 if(AT(unsigned char,other,0x7BAB)) force=9604.0f;
 scale(&delta,&delta,force);
 if((unsigned char)func_00138560(physics)) { scale(&temp,&delta,0.5f); func_00231CC0((ImpulseBody *)(physics), (Vector4 *)(&temp), (Vector4 *)(&AT(struct Body *,self,0x792C)->pos)); }

 else func_00231CC0((ImpulseBody *)physics,(Vector4 *)&delta,(Vector4 *)&AT(struct Body *,self,0x792C)->pos);
 if(force<3600.0f && D_004B25A0.mode==15) force=3601.0f;
 if(force>3600.0f) {
  float f;
  if(kind==3 || kind==8) func_00155E78(self+0x3FD0,8);
  else if(kind==4 || kind==9) func_00155E78(self+0x3FD0,9);
  else func_00155E78(self+0x3FD0,5);
  f=force;
  if(AT(unsigned char,other,0x7BAB)) f*=10.0f;
  norm(&temp,direction);
  scale(&temp,&temp,f*0.25f);
  func_001A77E0(self+0x4EF0,&temp);
 } else {
  struct Event *event=(struct Event *)(self+0x6268);
  float sign;
  event->xC=other;
  func_0012FE38(self,event);
  if(dot(&delta,physics->direction)<0.0f) sign=-1.0f; else sign=1.0f;
  func_001B0AC0(self+0x4E54,force*sign*0.00027777778f);
 }
 if(AT(int,self,0x7A5C)==3) {
  char *p=AT(char *,self,0x78D4);
  if(p) {
   if(AT(int,other,0x7A5C)==1) { int id=AT(int,other,0x7978); func_001126A8(p,id,func_00117470(p+0x2D4)); }
   else if(AT(int,other,0x7A5C)==AT(int,self,0x7A5C)) { int id=AT(int,other,0x7978); func_00112700(p,id,func_00117470(p+0x2D4)); }
  }
 }
 if(D_004B25A0.mode!=10 && AT(int,other,0x7A5C)==3) {
  char *p=AT(char *,other,0x78D4);
  if(p) {
   if(AT(int,self,0x7A5C)==1) func_00112808(p,AT(int,self,0x7978),10);
   else func_00112878(p,AT(int,self,0x7978),10);
  }
 }
 if(AT(int,other,0x7A5C)==1) func_001C7A60(other+0x7720,3,0.0f);
 else func_001C7A60(other+0x7720,16,0.0f);
 func_001D0A58(self+0x3FA8,kind,(int)force);
 if(kind==1 || kind==6) func_001301E0(other,1);
 else if(kind==2 || kind==7) func_001301E0(other,2);
 else if(kind==3 || kind==8) func_001301E0(other,3);
 else if(kind==4 || kind==9) func_001301E0(other,4);
 else func_001301E0(other,0);
 AT(int,self,0x79AC)++;
 if(D_004B25A0.mode==15 && greater(1,AT(int,self,0x79AC)) && AT(void *,self,0x4ECC)!=self+0x4EE0) func_00155E78(self+0x3FD0,9);
 {
  int type=AT(int,self,0x7A5C);
  if(type==1) {
   func_00254544(name,func_00158650(AT(int,self,0x79A8)));
   if(AT(int,self,0x79A8)==0xC0) func_00254544(name,func_00158668(5));
   func_002542C4(name,D_0029C298);
   if(kind==type || kind==6) {
    animate(D_003B2270,AT(unsigned int,self,0x7988),0x30,10,name);
   } else if(kind==0 || kind==5) {
    animate(D_003B2270,AT(unsigned int,self,0x7988),0x2F,11,name);
   }
  }
 }
}
