/* UNIT 0x00222660 0x00222690 */
typedef float Vector __attribute__((vector_size(16)));
extern float func_00222690(const void *);
float func_00222660(const void *a, const void *b) {
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
    return func_00222690(&difference);
}
