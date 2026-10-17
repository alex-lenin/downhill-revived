/* UNIT 0x00122D10 0x00122DC8 */
struct Body { char pad[0x10]; float x10[4]; };
class Actor {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};
struct Rider { char pad[0x792c]; Body *x792c; };
struct Obj { char pad0[0x130]; Actor *x130; char pad1[8]; Rider *x13c; };

/* returns 1 if obj's actor is within dist of its rider's body */
extern "C" int func_00122D10(Obj *obj, float dist) {
    float diff[4] __attribute__((aligned(16)));
    float distSq;
    float *pos;
    float *other;
    int inRange = 0;
    if(!obj->x13c) return 0;
    pos=obj->x130 ? obj->x130->v4()->x10 : 0;
    other=obj->x13c->x792c->x10;
    __asm__ volatile("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%2)" : : "r"(pos),"r"(other),"r"(diff) : "memory");
    __asm__("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=f"(distSq) : "f"(diff[0]),"f"(diff[1]),"f"(diff[2]));
    if (distSq < dist * dist) inRange = 1;
    return inRange;
}
