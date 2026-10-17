/* UNIT 0x001331E0 0x00133260 */
struct Obj { float x0; char pad4[0x34]; float *x38; char pad3c[0x24]; float x60[4]; char pad70[0x50]; float xc0[4]; };
void func_001331E0(struct Obj *obj, float *delta) {
 float length;
 float tmp[4] __attribute__((aligned(16)));
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x60(%0)" : : "r"(obj),"r"(obj->x60),"r"(delta) : "memory");
 __asm__("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(length) : "f"(delta[0]),"f"(delta[1]),"f"(delta[2]) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0(%0)" : : "r"(tmp),"r"(obj->x38+4),"f"(length) : "$2","memory");
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0x60(%0)" : : "r"(obj),"r"(obj->x60),"r"(delta) : "memory");
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,0xc0(%0)" : : "r"(obj),"r"(obj->x60),"f"(obj->x0) : "$2","memory");
}
