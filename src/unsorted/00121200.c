/* UNIT 0x00121200 0x00121348 */
typedef struct Vec {float x,y,z,w;} Vec __attribute__((aligned(16)));
typedef struct Matrix {Vec v[4];} Matrix;
typedef struct Target {char pad0[0x40]; Matrix x40;} Target;
typedef struct Obj {char pad0[0x14]; Target *x14;} Obj;
#define COPY_VECTOR(dst, src) __asm__ volatile("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline void Normalize(Vec *v) {
 float length,inv,x=v->x,y=v->y,z=v->z;
 __asm__ volatile("mula.s %1, %1
	madda.s %2, %2
	madd.s %0, %3, %3" : "=f"(length) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f),"f"(length));
 v->x=x*inv;v->y=y*inv;v->z=z*inv;
}
static inline void Cross(Vec *out,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11, 0x0(%1)
	lqc2 $vf12, 0x0(%2)
	vopmula.xyz ACC, $vf11, $vf12
	vopmsub.xyz $vf11, $vf12, $vf11
	sqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b));
}
static inline void CopyMatrix(Matrix *out, Matrix *in) {
 __asm__ volatile("lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3" : "=m"(out->v[0]),"=m"(out->v[1]),"=m"(out->v[2]),"=m"(out->v[3]) : "r"(in) : "$8","$9","$10","$11","memory");
}
void func_00121200(Obj *obj, Vec *up,Vec *direction) {
 Matrix matrix;
 COPY_VECTOR(matrix.v[2],direction);
 Normalize(&matrix.v[2]);
 Cross(&matrix.v[0],up,&matrix.v[2]);
 Normalize(&matrix.v[0]);
 Cross(&matrix.v[1],&matrix.v[2],&matrix.v[0]);
 Normalize(&matrix.v[1]);
 if(obj->x14) {
  CopyMatrix(&obj->x14->x40,&matrix);
  obj->x14->x40.v[3].x=0.0f;
  obj->x14->x40.v[3].y=0.0f;
  obj->x14->x40.v[3].z=0.0f;
  obj->x14->x40.v[3].w=1.0f;
  obj->x14->x40.v[0].w=0.0f;
  obj->x14->x40.v[1].w=0.0f;
  obj->x14->x40.v[2].w=0.0f;
 }
}
