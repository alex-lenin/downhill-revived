/* UNIT 0x00211828 0x00211858 */
void func_00211828(void *out, const void *vectors, const void *offset) {
    __asm__ volatile (
        "lqc2 $vf14, 0x0(%1)
	"
        "lqc2 $vf15, 0x10(%1)
	"
        "lqc2 $vf16, 0x20(%1)
	"
        "lqc2 $vf10, 0x0(%2)
	"
        "vadd.xyz $vf14, $vf14, $vf10
	"
        "vadd.xyz $vf15, $vf15, $vf10
	"
        "vadd.xyz $vf16, $vf16, $vf10
	"
        "sqc2 $vf14, 0x0(%0)
	"
        "sqc2 $vf15, 0x10(%0)
	"
        "sqc2 $vf16, 0x20(%0)"
        : : "r"(out), "r"(vectors), "r"(offset) : "memory");
}
