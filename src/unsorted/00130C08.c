/* UNIT 0x00130C08 0x00130D80 */
typedef float Vector[4] __attribute__((aligned(16)));
typedef struct Body { float mass; char pad4[0x4c]; Vector center; char pad60[0x20]; float matrix[16]; } Body;
extern void func_00221888(float *, float *, float *);
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b) : "memory")
#define CROSS(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b) : "memory")
#define DOT(dst,a,b) __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(dst) : "f"((a)[0]),"f"((b)[0]),"f"((a)[1]),"f"((b)[1]),"f"((a)[2]),"f"((b)[2]))
float func_00130C08(Body *body, float *a, float *b, float *point, float *normal, float *out, float bounce) {
    Vector r,relative,cross,transformed;
    float speed,impulse,mass,dot;
    __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(out) : "memory");
    SUB(r,point,body->center);
    SUB(relative,a,b);
    DOT(speed,normal,relative);
    if(speed>0.001f) return 0.0f;
    if(speed>-0.001f) return 0.0f;
    impulse = -(bounce+1.0f)*speed;
    mass=body->mass;
    CROSS(cross,r,normal);
    func_00221888(body->matrix,cross,transformed);
    CROSS(cross,transformed,r);
    DOT(dot,normal,cross);
    mass+=dot;
    impulse/=mass;
    __asm__ volatile("lqc2 $vf11,0(%0)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%1)" : : "r"(normal),"r"(out),"f"(impulse) : "$2","memory");
    return impulse;
}
