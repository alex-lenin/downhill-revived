/* UNIT 0x00134D70 0x00134DD8 */
typedef struct { char pad[0x34]; void *x34,*x38; } Obj;
extern void func_00134D60(void *,void *);
void func_00134D70(void *src,void *dst,Obj *o) {
 void *m,*p;
 func_00134D60(src,dst);
 m=o->x38;
 __asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tlqc2 $vf13,0x10(%1)\n\tlqc2 $vf14,0x20(%1)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,0(%0)" : : "r"(dst),"r"(m) : "memory");
 p=o->x34;
 __asm__ volatile ("lqc2 $vf11,0(%0)\n\tlqc2 $vf12,0(%1)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(dst),"r"(p) : "memory");
}
