/* UNIT 0x0012AF48 0x0012B188 */
extern float fabsf(float);
static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
typedef float Vec[4] __attribute__((aligned(16)));
typedef struct { Vec x,y,z,w; } LocalMatrix __attribute__((aligned(16)));
struct Matrix { Vec v[4]; } __attribute__((aligned(16)));
typedef struct { char pad[4]; Vec *x4; } Obj;
static inline void Cross(Vec *d, const Vec *a, const Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a),"r"(b));
}
#define CROSS(d,a,b) Cross(&(d),a,b)
#define COPY(d,s) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(d) : "r"(s) : "$2")
#define MATRIX_COPY(d,s) __asm__ volatile("lq $8,0(%1)\n\tlq $9,16(%1)\n\tlq $10,32(%1)\n\tlq $11,48(%1)\n\tsq $8,0(%0)\n\tsq $9,16(%0)\n\tsq $10,32(%0)\n\tsq $11,48(%0)" : : "r"(d),"r"(s) : "$8","$9","$10","$11","memory")
static inline float normalize(Vec *v) {
 float x,y,z,n,r; x=(*v)[0]; y=(*v)[1]; z=(*v)[2];
 __asm__ volatile("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3" : "=f"(n) : "f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(r) : "f"(1.0f),"f"(n));
 (*v)[0]=x*r; (*v)[1]=y*r; (*v)[2]=z*r;
 return n;
}
static inline Vec *normalize_next(Vec *v) {
 normalize(v);
 return v+1;
}
void func_0012AF48(Obj *o,Vec *a,Vec *b) {
 Vec up; LocalMatrix m; Vec *dst; Vec *v; float az=(*a)[2];
 v=&up; (*v)[0]=0; (*v)[2]=1; (*v)[1]=0;
 v=a;
 dst=o->x4+4;
 if (Abs(az)<0.995f) { CROSS(m.x,a,&up); }
 else if (Abs((*b)[2])<0.995f) { CROSS(m.x,a,b); }
 else if (Abs(dst[2][2])<0.995f) { CROSS(m.x,a,dst+2); }
 else { COPY(m.x,dst); }
 o=(Obj *)normalize_next(&m.x);
 CROSS(m.y,b,&m.x);
 if(normalize(&m.y)<0.005f) { CROSS(m.y,&up,b); normalize(&m.y); }
 CROSS(m.z,&m.x,(Vec *)o); normalize(&m.z);
 MATRIX_COPY(dst,&m);
 __asm__ volatile("sqc2 $vf0,0x30(%0)" : : "r"(dst) : "memory");
 dst[0][3]=dst[1][3]=dst[2][3]=0.0f;
}
