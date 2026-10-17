/* UNIT 0x00123DB0 0x00123F60 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Obj { char pad[0x100]; Vec x100; char pad110[0xF0]; Vec x200; };
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
static inline void Add(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
static inline void Cross(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
static inline float Length(Vec *v) {
 float r;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(r) : "f"(v->x),"f"(v->y),"f"(v->z));
 return r;
}
static inline void Norm(Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(len));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
static inline void NormSqrt(Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv; int bits;
 /* Sony length/reciprocal wrapper, also used in 00137208. */
 __asm__ volatile("mula.s %3,%3
	madda.s %4,%4
	madd.s %1,%5,%5
	sqrt.s %1,%1
	lui %2,0x3f80
	mtc1 %2,%0
	nop
	div.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(x),"f"(y),"f"(z));
 v->x=x*inv; v->y=y*inv; v->z=z*inv;
}
static inline void SetVec(Vec *v,float x,float y,float z) { v->x=x; v->y=y; v->z=z; }
void func_00123DB0(struct Obj *obj,Vec *out) {
 Vec a,b; float w; Vec *forward, *up;
 a.x=0; a.y=0; a.z=1; a.w=0;
 Add(&a,&a,&obj->x100); Norm(&a); forward=&out[2]; COPY_VECTOR(*forward,&a);
 up=&obj->x200; COPY_VECTOR(b,up);
 if(Length(&b)<0.01f) {
  SetVec(&b,0,1,0); w=up->w;
  COPY_VECTOR(obj->x200,&b); up->w=w;
 }
 Cross(&out[0],up,forward); NormSqrt(&out[0]);
 Cross(&out[1],forward,&out[0]); NormSqrt(&out[1]);
 out[2].w=0; out[3].x=0; out[3].y=0; out[3].z=0;
 out[0].w=0; out[1].w=0; out[3].w=1;
}
