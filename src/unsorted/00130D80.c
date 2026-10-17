/* UNIT 0x00130D80 0x00130FE0 */
typedef struct { float x,y,z,w; } Vec;
typedef struct { int enabled, x4; float value; } Coeff;
typedef struct { float mass; char pad4[0x4c]; Vec x50; char pad60[0x100]; Coeff x160; } Obj;
extern void func_00221888(void *,Vec *,Vec *);
static inline float dot(Vec *a,Vec *b) {
 float r;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
 return r;
}
static inline void scale(Vec *d,Vec *s,float f) {
 __asm__ volatile("lqc2 $vf11,0(%1)
	mfc1 $2,%2
	qmtc2.ni $2,$vf12
	vmulx.xyz $vf11,$vf11,$vf12x
	sqc2 $vf11,%0" : "=m"(*d) : "r"(s),"f"(f) : "$2");
}
static inline void sub(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
static inline void addscale(Vec *d,Vec *a,Vec *b,float f) {
 int bits;
 __asm__ volatile("lqc2 $vf11,0(%2)\n\tlqc2 $vf12,0(%3)\n\tmfc1 %0,%4\n\tqmtc2 %0,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%1" : "=&r"(bits),"=m"(*d) : "r"(a),"r"(b),"f"(f));
}
static inline void cross(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
static inline float normalize(Vec *a) {
 float r,inv;
 __asm__ volatile ("mula.s %2, %2
	madda.s %3, %3
	madd.s %1, %4, %4
	sqrt.s %1, %1
	"
 "lui $2, 0x3f80
	mtc1 $2, %0
	nop
	div.s %0, %0, %1"
 : "=&f"(inv), "=&f"(r) : "f"(a->x), "f"(a->y), "f"(a->z), "f"(a->z) : "$2");
 a->x*=inv; a->y*=inv; a->z*=inv; return r;
}
static inline float normalize2(Vec *a) {
 float r,inv;
 __asm__ volatile ("mula.s %2, %2
	madda.s %3, %3
	madd.s %1, %4, %4
	sqrt.s %1, %1
	"
 "lui $2, 0x3f80
	mtc1 $2, %0
	nop
	div.s %0, %0, %1"
 : "=&f"(inv), "=&f"(r) : "f"(a->x), "f"(a->y), "f"(a->z), "f"(a->z), "f"(a->x), "f"(a->y) : "$2");
 a->x*=inv; a->y*=inv; a->z*=inv; return r;
}
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline float coefficient(Coeff *c) { if(c->enabled) return c->value; return 1.0f; }
void func_00130D80(Obj *o,Vec *velocity,Vec *pos,Vec *normal,Vec *direction,Vec *out) {
 Vec a,b,c,d; float numerator,length,mass;
 __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(out) : "memory");
 scale(&a,normal,dot(normal,velocity)); sub(&a,velocity,&a);
 COPY_VECTOR(b,direction); addscale(&b,&b,normal,-dot(&b,normal));
 normalize2(&b); scale(&a,&b,dot(&b,&a)); length=normalize(&a); scale(&a,&a,-1.0f);
 sub(&b,pos,&o->x50); { float f=coefficient(&o->x160);
 numerator=-(f+-1.0f+1.0f)*length; mass=o->mass; }
 cross(&c,&b,&a);func_00221888((char *)o+0x80,&c,&d);cross(&c,&d,&b);
 numerator=numerator/(mass+dot(&a,&c)); COPY_VECTOR(*out,&a);out->w=numerator;
}
