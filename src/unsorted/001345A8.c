/* UNIT 0x001345A8 0x00134608 */
struct Transform { char pad[0x34]; float *x34; float *x38; };
void func_001345A8(char *obj, float *dst, float *src) {
    struct Transform *t = (struct Transform *)(obj + 0x10);
    float x = src[0], y = src[1], z = src[2];
    dst[0] = x; dst[1] = y; dst[2] = z;
    __asm__ volatile ("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	lqc2 $vf13,16(%1)
	lqc2 $vf14,32(%1)
	vmulax.xyz ACC,$vf12,$vf11x
	vmadday.xyz ACC,$vf13,$vf11y
	vmaddz.xyz $vf11,$vf14,$vf11z
	sqc2 $vf11,0(%0)" : : "r"(dst), "r"(t->x38) : "memory");
    __asm__ volatile ("lqc2 $vf11,0(%0)
	lqc2 $vf12,0(%1)
	vadd.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,0(%0)" : : "r"(dst), "r"(t->x34) : "memory");
}
