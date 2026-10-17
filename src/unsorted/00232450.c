/* UNIT 0x00232450 0x002325E0 */
typedef struct Vec {float x,y,z,w;} Vec __attribute__((aligned(16)));
struct Body {float x0; char pad4[0x4c]; Vec x50; char pad60[0x20]; float x80[4][4]; Vec xC0; Vec xD0;};
extern void func_00221888(void *,Vec *,Vec *);
static inline void Sub(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out):"r"(a),"r"(b));
}
static inline void Add(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out):"r"(a),"r"(b));
}
static inline void Cross(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*out):"r"(a),"r"(b));
}
static inline float Dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(r):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline void Scale(Vec *out,Vec *a,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"f"(scale):"$2");
}
void func_00232450(struct Body *body,Vec *out,Vec *velocity,Vec *point,Vec *normal,float bounce) {
 Vec relative, contact, delta;
 float dot, mass, denominator, impulse;
 Sub(&relative,point,&body->x50);
 Cross(&contact,&body->xD0,&relative);
 Add(&contact,&contact,&body->xC0);
 Sub(&delta,&contact,velocity);
 dot=Dot(normal,&delta);
 if(dot > -0.001f) {
  __asm__ volatile("sqc2 $vf0,0(%0)"::"r"(out):"memory");
 } else {
  mass=body->x0;
  Cross(&delta,&relative,normal);
  func_00221888(body->x80,&delta,&delta);
  Cross(&delta,&delta,&relative);
  denominator=mass+Dot(normal,&delta);
  impulse=(-(bounce+1.0f)*dot)/denominator;
  out->w=impulse;
  Scale(out,normal,impulse);
 }
}
