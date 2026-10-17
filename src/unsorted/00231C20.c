/* UNIT 0x00231C20 0x00231C40 */
void func_00231C20(char *obj, void *offset) {
 void *pos = obj + 0xE0;
 __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0xE0(%2)" : : "r"(pos), "r"(offset), "r"(obj) : "memory");
}
