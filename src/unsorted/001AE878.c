/* UNIT 0x001AE878 0x001AE8A8 */
void func_001AE878(char *p) {
    int i = 3;
    p += 0x20;
    do {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(p) : "memory");
        i--;
        p += 0x10;
    } while(i >= 0);
}
