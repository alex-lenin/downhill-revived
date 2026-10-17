/* UNIT 0x0016ABC8 0x0016ACE8 */
typedef float Vec4[4] __attribute__((aligned(16)));
typedef struct Obj {int x0;void *x4;int x8;float xC;char pad10[16];Vec4 x20;Vec4 x30;Vec4 x40;} Obj;
extern char D_0026EBE8[];
extern float func_0016AD98(void *,float);
extern void func_00231D88(void *,float *,float *);
static inline void ScaleVector(Vec4 out, Vec4 in,float scale) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2.ni $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(*(Vec4 *)out) : "r"(in),"f"(scale) : "$2","memory");
}
static inline float DotProduct(float *a,float *b) {float result;
 __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2]));
 return result;
}
static inline void SetVector(float *v,float x,float y,float z,float w) {v[0]=x;v[1]=y;v[2]=z;v[3]=w;}
void func_0016ABC8(Obj *obj) {
 Vec4 force; Vec4 position;float dot,scale;
 dot=DotProduct(obj->x40,obj->x20);
 force[0]=1.0f;((int *)force)[1]=0;((int *)force)[2]=0;((int *)force)[3]=0;
 scale=func_0016AD98(D_0026EBE8,obj->xC);
 if(dot>0.0f) scale=-scale;
 ScaleVector(force,force,scale);
 SetVector(position,0.0f,-3.0f,0.0f,0.0f);
 func_00231D88(obj->x4,force,position);
 ScaleVector(force,force,-1.0f);
 ScaleVector(position,position,-1.0f);
 func_00231D88(obj->x4,force,position);
}
