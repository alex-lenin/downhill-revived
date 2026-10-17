/* UNIT 0x0017D410 0x0017D420 */
void func_0017D410(void *arg0, void *arg1, void *dest)
{
    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(dest) : "memory");
}
