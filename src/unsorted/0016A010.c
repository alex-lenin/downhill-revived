/* UNIT 0x0016A010 0x0016A1F8 .sdata=0x29C860:0x29C864 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00169F50_11_func_00169F50 func_00169F50
#else
#define CallScalarBridge00169F50_11_func_00169F50 ScalarBridge00169F50_11
#endif
typedef struct { float x,y,z,w; } Vec;
typedef struct { char pad0[0x20]; Vec x20; } Basis;
typedef struct { char pad0[0x38]; Basis *x38; char pad3c[0x84]; Vec xc0; char padd0[0x3d6]; short x4a6; } Rider;
typedef struct Scalar0016A010Obj { int x0; Rider *x4; float x8; char padc[4]; Vec x10,x20,x30,x40; } Obj;
extern float D_0029C854,D_0029C858;
float D_0029C860=40.0f;
extern char D_0026EBD0[];
extern int func_00138570(Rider *);
extern float func_00248B90(float),func_0016AD98(void *,float);

#if defined(__mips__)
extern void func_00169F50(Obj *,float,float);
#else

#endif

static inline float dot(Vec *a,Vec *b) {
    float out;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(out):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return out;
}
static inline float dotX(Vec *a,Vec *b) {
    float out=a->x;
    __asm__ volatile ("mula.s %0,%1
	madda.s %2,%3
	madd.s %0,%4,%5" : "+f"(out):"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return out;
}
static inline float length(Vec *v) {
    float result;
    __asm__ volatile ("mula.s %1,%1
	madda.s %2,%2
	madd.s %0,%3,%3
	sqrt.s %0,%0" : "=&f"(result):"f"(v->x),"f"(v->y),"f"(v->z));
    return result;
}
int func_0016A010(Obj *obj) {
    float first,second,angle,value;
    Vec *dir;
    if (!func_00138570(obj->x4)) return 0;
    if ((float)obj->x4->x4a6<D_0029C854) return 0;
    if (length(&obj->x4->xc0)<D_0029C860) return 0;
    dir=&obj->x40;
    first=func_00248B90(dot(dir,&obj->x30))*0.63661974668502807617f;
    if(first>1.0f) first=1.0f;
    second=func_00248B90(dot(&obj->x10,&obj->x4->x38->x20))*0.63661974668502807617f;
    if(second>1.0f) second=1.0f;
    __asm__ volatile("max.s %0,%1,%2":"=&f"(angle):"f"(first),"f"(second));
    value=func_0016AD98(D_0026EBD0,angle);
    if(obj->x0 == 0 || value<=obj->x8) {
    value=dotX(dir,&obj->x20);
    D_0029C858=(float)obj->x4->x4a6;
    CallScalarBridge00169F50_11_func_00169F50(obj,angle,value);
    return 1;
    }
    return 0;
}
