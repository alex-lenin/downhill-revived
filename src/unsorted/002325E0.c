/* UNIT 0x002325E0 0x002327B0 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Body { float x0; char pad4[0x4c]; Vec x50; char pad60[0x20]; char x80[0x40]; Vec xC0; Vec xD0; } Body;
extern void func_00221888(void *,Vec *,Vec *);
#define OP(out,a,b,op) __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\t" op "\n\tsqc2 $vf11, %0" : "=m"(*(out)):"r"(a),"r"(b))
#define OPP(out,a,b,op) __asm__ volatile("lqc2 $vf11, 0(%1)
	lqc2 $vf12, 0(%2)
	" op "
	sqc2 $vf11, %0" : "=m"(*(out)):"r"(a),"r"(b),"r"(out))
#define CROSS(out,a,b) OPP(out,a,b,"vopmula.xyz ACC, $vf11, $vf12\n\tvopmsub.xyz $vf11, $vf12, $vf11")
static inline float Dot(Vec *a, Vec *b) { float r; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6":"=f"(r):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r; }
static inline void Scale(Vec *out,Vec *in,float s) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0":"=m"(*out):"r"(in),"f"(s):"$2"); }
static inline void Reject(Vec *out,Vec *a,Vec *b,float s) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tmfc1 $2, %3\n\tqmtc2 $2, $vf13\n\tvaddax.xyz ACC, $vf11, $vf0x\n\tvmsubx.xyz $vf11, $vf12, $vf13x\n\tsqc2 $vf11, %0":"=m"(*out):"r"(a),"r"(b),"f"(s):"$2"); }
void func_002325E0(Body *body,Vec *out,Vec *velocity,Vec *pos,Vec *normal) {
 Vec r, v, relative, tangent, cross; float d,len,mass,impulse;
 OP(&r,pos,&body->x50,"vsub.xyz $vf11, $vf11, $vf12");
 __asm__ volatile("lqc2 $vf11, 0(%1)
	lqc2 $vf12, 0(%2)
	vopmula.xyz ACC, $vf11, $vf12
	vopmsub.xyz $vf11, $vf12, $vf11
	sqc2 $vf11, %0":"=m"(v):"r"(&body->xD0),"r"(&r));
 OP(&v,&v,&body->xC0,"vadd.xyz $vf11, $vf11, $vf12");
 OPP(&relative,&v,velocity,"vsub.xyz $vf11, $vf11, $vf12");
 d=Dot(normal,&relative);
 if(d > -0.001f) { __asm__ volatile("sqc2 $vf0, %0":"=m"(*out)); return; }
 Reject(&tangent,&relative,normal,d);
 __asm__ volatile("mula.s %1, %1
	madda.s %2, %2
	madd.s %0, %3, %3
	sqrt.s %0, %0":"=&f"(len):"f"(tangent.x),"f"(tangent.y),"f"(tangent.z));
 tangent.w=len;
 Scale(&tangent,&tangent,1.0f/len);
 mass=body->x0;
 CROSS(&cross,&r,&tangent);
 func_00221888(body->x80,&cross,&cross);
 CROSS(&cross,&cross,&r);
 mass+=Dot(&tangent,&cross);
 impulse=tangent.w/mass;
 out->w=impulse;
 Scale(out,&tangent,-impulse);
}
