/* UNIT 0x002218D0 0x00221930 */
void func_002218D0(const float *matrix, const float *vector, float *out) {
    __asm__ volatile (
        "lqc2       $vf11, 0x0(%1)
	"
        "lqc2       $vf12, 0x0(%0)
	"
        "lqc2       $vf13, 0x10(%0)
	"
        "lqc2       $vf14, 0x20(%0)
	"
        "lqc2       $vf15, 0x30(%0)
	"
        "vmul.xyz   $vf12, $vf12, $vf11
	"
        "vmul.xyz   $vf13, $vf13, $vf11
	"
        "vmul.xyz   $vf14, $vf14, $vf11
	"
        "vmul.xyz   $vf15, $vf15, $vf11
	"
        "vaddw.z    $vf12, $vf12, $vf12w
	"
        "vaddx.w    $vf13, $vf13, $vf13x
	"
        "vaddy.x    $vf14, $vf14, $vf14y
	"
        "vaddz.y    $vf15, $vf15, $vf15z
	"
        "vaddz.y    $vf12, $vf12, $vf12z
	"
        "vaddw.z    $vf13, $vf13, $vf13w
	"
        "vaddx.w    $vf14, $vf14, $vf14x
	"
        "vaddy.x    $vf15, $vf15, $vf15y
	"
        "vaddy.x    $vf11, $vf12, $vf12y
	"
        "vaddz.y    $vf11, $vf13, $vf13z
	"
        "vaddw.z    $vf11, $vf14, $vf14w
	"
        "vaddx.w    $vf11, $vf15, $vf15x
	"
        "sqc2       $vf11, 0x0(%2)
	"
        : : "r"(matrix), "r"(vector), "r"(out) : "memory");
}
