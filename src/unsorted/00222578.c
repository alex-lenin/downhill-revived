/* UNIT 0x00222578 0x002225A8 */
typedef float Vector __attribute__((vector_size(16)));
extern void func_002225A8(void *, const void *);
void func_00222578(void *out, const void *a, const void *b) {
    Vector difference;
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)
	"
        "lqc2 $vf12, 0x0(%2)
	"
        "vsub.xyz $vf11, $vf11, $vf12
	"
        "sqc2 $vf11, %0"
        : "=m"(difference) : "r"(b), "r"(a));
    func_002225A8(out, &difference);
}
