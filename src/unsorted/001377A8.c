/* UNIT 0x001377A8 0x00137950 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec { u128 q; float f[4]; } Vec;
typedef struct Obj { char pad0[0x38]; float *matrix; char pad3c[0x244-0x3c]; float x244; char pad248[0x44c-0x248]; float x44c; } Obj;
extern float func_00248B90(float);
extern float func_0016B060(void *);
static inline float Dot(float *a,float *b) {
    float d;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(d) : "f"(a[0]),"f"(b[0]),"f"(a[1]),"f"(b[1]),"f"(a[2]),"f"(b[2]) : "memory");
    return d;
}
void func_001377A8(Obj *obj,Vec *in) {
    Vec cross, dir;
    float *m = obj->matrix;
    float *up=m+4, *forward=m+8;
    float d, x,y,z,length,inv,angle,t,limit;
    __asm__ volatile("lq $2,0(%1)
	sq $2,%0" : "=m"(dir) : "r"(in) : "$2");
    d=Dot(up,dir.f);
    if (d < 0.9f) {
        t=-d;
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(dir) : "r"(&dir),"r"(up),"f"(t) : "$2");
        x=dir.f[0]; y=dir.f[1]; z=dir.f[2];
        __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(length) : "f"(x),"f"(y),"f"(z));
        __asm__ volatile("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f),"f"(length));
        dir.f[0]=x*inv; dir.f[1]=y*inv; dir.f[2]=z*inv;
        angle=func_00248B90(Dot(forward,dir.f))*0.5f;
        limit=obj->x44c*func_0016B060((char *)obj+0x160);
        __asm__ volatile("min.s %0,%1,%2" : "=&f"(t) : "f"(angle),"f"(limit));
        angle=t;
        __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(cross) : "r"(forward),"r"(&dir));
        if (Dot(cross.f,up) < 0.0f) angle=-angle;
        obj->x244=-angle;
    } else obj->x244=0.0f;
}
