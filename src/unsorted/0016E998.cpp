/* UNIT 0x0016E998 0x0016EB40 */
typedef struct { float x,y,z,w; } Vec;
class Model { public: virtual void v1(); virtual void v2(); virtual void v3(); virtual char *get(); };
typedef struct { char pad0[0x130]; Model *x130; char pad134[0x9c]; Vec x1d0; char pad1e0[16]; Vec x1f0; } Obj;
extern void func_0016E8D8(Obj *,Obj *,Vec *);
static inline Vec *position(Obj *obj) {
    Model *m=obj->x130;
    if(m) return (Vec *)(m->get()+16);
    return 0;
}
static inline void sub(Vec *out,Vec *a,Vec *b) {
    __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*out):"r"(a),"r"(b));
}
static inline void normalize(Vec *v) {
    float length,inv,x=v->x,y=v->y,z=v->z;
    __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(length):"f"(x),"f"(y),"f"(z));
    __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(length));
    v->x=x*inv;v->y=y*inv;v->z=z*inv;
}
static inline float dot(Vec *a,Vec *b) {
    float out;
    __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=f"(out):"f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z));
    return out;
}
int func_0016E998(Obj *a,Obj *b) {
    Vec first,second;
    Vec *p,*q,*axis;
    Model *m;
    int result=0;
    p=&a->x1f0;
    sub(&first,p,position(b));
    normalize(&first);
    func_0016E8D8(a,b,&first);
    q=&b->x1d0;
    sub(&second,q,position(b));
    normalize(&second);
    m=b->x130;
    axis=(Vec *)(m->get()+0x50);
    if(dot(&second,axis)>0.7f) result=1;
    return result;
}
