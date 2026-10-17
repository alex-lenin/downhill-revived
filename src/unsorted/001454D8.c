/* UNIT 0x001454D8 0x00145768 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Frame { int *x0; char pad4[12]; Vec x10; char pad20[48]; Vec x50; } Frame;
typedef struct Obj { char pad0[8]; Frame *x8; char padc[0x184]; float x190,x194; } Obj;
extern int func_00211390(Vec *,int,int,Frame **,int,float);
extern float func_00235EA8(float,float,float,float);
extern void func_00145E08(Obj *,float);
static inline void Sub(Vec *out,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b)); }
static inline float Dot(Vec *a,Vec *b) { float r; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r; }
static inline void Normalize(Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv; int bits;
 /* Sony length/reciprocal sequence (sqrt.s, then 1.0f/len) */
 __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
static inline void Update(Obj *obj,float target,float rate) { float value=func_00235EA8(obj->x190,target,rate,0.001f); obj->x190=value; func_00145E08(obj,value); }
void func_001454D8(Obj *obj) {
 Frame *nearby[3];
 Vec delta;
 int count,i;
 count=func_00211390(&obj->x8->x10,0x51e,0x528,nearby,3,30.0f);
 if(count>=2) {
  for(i=0;i<count;i++) {
   Frame *other=nearby[i],*self=obj->x8;
   if((*other->x0&0x3ff80)!=(*self->x0&0x3ff80)) {
    Sub(&delta,&self->x10,&other->x10);
    Normalize(&delta);
    if(Dot(&self->x50,&delta)>0.0f && Dot(&self->x50,&nearby[i]->x50)>0.0f) {
     Update(obj,obj->x194+0.12222222f,0.08f);
    } else if(Dot(&obj->x8->x50,&nearby[i]->x50)>0.0f) {
     Update(obj,0.04888889f,0.01f);
    } else Update(obj,obj->x194,0.01f);
   }
  }
 } else Update(obj,obj->x194,0.01f);
}
