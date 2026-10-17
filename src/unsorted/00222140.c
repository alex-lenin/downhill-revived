/* UNIT 0x00222140 0x00222168 */
typedef unsigned int u128 __attribute__((mode(TI)));
void func_00222140(volatile u128 *out, void *in) {
    u128 value;
    __asm__ volatile ("lqc2 $vf14, 0(%1)
	vcallms 0x830
	nop
	qmfc2.i %0, $vf10
	sqc2 $vf11, 0x10(%2)
	sqc2 $vf12, 0x20(%2)" : "=&r"(value) : "r"(in), "r"(out) : "memory");
    *out = value;
}
