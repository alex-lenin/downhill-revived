/* UNIT 0x00170278 0x001702C8 */
extern int func_00238F30(void);
void func_00170278(char *obj, int unused, float *out) {
 float *pos=(float *)(obj+0x20);
 float scale=(float)func_00238F30();
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%1)" : : "r"(pos), "r"(out), "f"(scale) : "$2", "memory");
}
