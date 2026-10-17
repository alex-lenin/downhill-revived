/* UNIT 0x0021EAA0 0x0021EAF0 -O0 -fno-opt-stack */
extern void func_0021E9A8(void);
void func_0021EAA0(int enabled) {
    if (enabled == 1) func_0021E9A8();
    __asm__ volatile ("sync\n\tei");
}
