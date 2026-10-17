/* UNIT 0x00231C10 0x00231C20 */
void func_00231C10(void *obj) {
    __asm__ volatile ("sqc2 $vf0, 0xE0(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0xF0(%0)" : : "r"(obj) : "memory");
}
