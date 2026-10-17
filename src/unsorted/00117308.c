/* UNIT 0x00117308 0x00117470 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Transform { char pad0[16]; Vec x10; char pad20[32]; Vec x40,x50; };
struct Rider { char pad0[0x792C]; struct Transform *x792C; char pad7930[0x3C]; int x796C; };
struct Link { int x0; struct Rider *x4; char pad8[0x438]; unsigned long x440; };
struct State { char pad0[0x58]; struct Rider *x58; struct Link *x5C; };
extern void func_00221888(Vec *,Vec *,Vec *);
static inline void Sub(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline float Dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z;
 __asm__ volatile("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(length) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f),"f"(length));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
static inline void Scale(Vec *out,Vec *v,float scale) {
 __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(v),"f"(scale) : "$2");
}
void func_00117308(struct State *s,Vec *out) {
 Vec delta,projection; struct Rider *other; struct Transform *t; Vec *axis;
 out->x=3.0f; out->y=15.0f; out->z=0.0f;
 if(s->x58->x796C==0xA0 || s->x58->x796C==0x160) {
  out->x=-2.0f;
  other=0;
  if(s->x5C->x440 & 0x10000000000UL) other=s->x5C->x4;
  if(other) {
   Sub(&delta,&s->x58->x792C->x10,&other->x792C->x10);
   t=other->x792C; axis=&t->x40;
   if(Dot(&t->x50,&delta)<out->z) {
    func_00221888(axis,&delta,&projection);
    Normalize(&projection); Scale(out,&projection,10.0f);
   }
  }
 }
}
