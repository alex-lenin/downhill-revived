/* UNIT 0x0023B140 0x0023B1B0 */
extern int DIntr(void);
extern void func_0025EEA0(void);
void func_0023B140(int value) {
 DIntr();
 *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 | 0x10000;
 *(volatile int *)0x1000B000 = value;
 *(volatile int *)0x1000F590 = *(volatile int *)0x1000F520 & 0xFFFEFFFF;
 func_0025EEA0();
}
