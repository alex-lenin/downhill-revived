/* UNIT 0x00212748 0x002128B8 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Shape { int flags; char pad[12]; Vec x10; };
struct Obj { union {struct Shape *shape; unsigned long flags;} x0; char pad[8]; Vec x10; char pad2[32]; Vec x40[3]; char pad3[32]; Vec x90; char pad4[32]; Vec xc0[3]; };
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Transform(Vec *out,Vec *v,Vec *m) {
 __asm__ volatile("lqc2 $vf11, %1\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,16(%2)\n\tlqc2 $vf14,32(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0" : "=m"(*out) : "m"(*v),"r"(m));
}
static inline void Add(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline float Max(float a,float b) {float v;__asm__ volatile("max.s %0,%1,%2":"=&f"(v):"f"(a),"f"(b));return v;}
void func_00212748(struct Obj *o,Vec *lo,Vec *hi) {
 Vec local,current,previous;float radius;struct Shape *shape=o->x0.shape;
 if((shape->flags&63)==1) {COPY_VECTOR(local,&shape->x10);radius=shape->x10.w;}else {__asm__ volatile("sqc2 $vf0,%0":"=m"(local));radius=10.0f;}
 Transform(&current,&local,o->x40);Add(&current,&current,&o->x10);
 if(o->x0.flags&0x400000000000000UL) {
 Transform(&previous,&local,o->xc0);Add(&previous,&previous,&o->x90);
 hi->x=Max(previous.x,current.x)+radius;hi->y=Max(previous.y,current.y)+radius;hi->z=Max(previous.z,current.z)+radius;
 lo->x=Max(previous.x,current.x)-radius;lo->y=Max(previous.y,current.y)-radius;lo->z=Max(previous.z,current.z)-radius;
 }else {hi->x=current.x+radius;hi->y=current.y+radius;hi->z=current.z+radius;lo->x=current.x-radius;lo->y=current.y-radius;lo->z=current.z-radius;}
}
