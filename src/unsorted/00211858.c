/* UNIT 0x00211858 0x00211888 */
void func_00211858(void *a, void *b, void *c, const void *scale) {
    __asm__ volatile (
        "lqc2 $vf14, 0x0(%0)
	"
        "lqc2 $vf15, 0x0(%1)
	"
        "lqc2 $vf16, 0x0(%2)
	"
        "lqc2 $vf10, 0x0(%3)
	"
        "vmul.xyz $vf14, $vf14, $vf10
	"
        "vmul.xyz $vf15, $vf15, $vf10
	"
        "vmul.xyz $vf16, $vf16, $vf10
	"
        "sqc2 $vf14, 0x0(%0)
	"
        "sqc2 $vf15, 0x0(%1)
	"
        "sqc2 $vf16, 0x0(%2)"
        : : "r"(a), "r"(b), "r"(c), "r"(scale) : "memory");
}
