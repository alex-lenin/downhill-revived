/* UNIT 0x00238B20 0x00238B40 */
extern int D_0029E3E0;

/* 0x10000800 = EE timer 1 count */
void func_00238B20(void)
{
    D_0029E3E0 = 0;
    *(volatile int *)0x10000800 = 0;
}
