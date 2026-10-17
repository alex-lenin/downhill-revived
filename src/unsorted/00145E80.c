/* UNIT 0x00145E80 0x00145ED0 */
extern int func_00238F30(void);
void func_00145E80(char *obj, int unused, void *out) {
 void *vec=obj+0x50;
 float scale=(float)func_00238F30();
 __asm__ volatile("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %2\n\tqmtc2 $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%1)" : : "r"(vec), "r"(out), "f"(scale) : "$2", "memory");
}
