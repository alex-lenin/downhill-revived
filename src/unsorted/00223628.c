/* UNIT 0x00223628 0x00223638 */
void func_00223628(void *a, void *b)
{
    __asm__ volatile ("sqc2 $vf30, 0x0(%0)" : : "r"(a) : "memory");
    __asm__ volatile ("sqc2 $vf31, 0x0(%0)" : : "r"(b) : "memory");
}
