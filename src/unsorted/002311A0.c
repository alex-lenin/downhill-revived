/* UNIT 0x002311A0 0x002311C8 */
extern void func_0025A710(int value);

/* Interrupt handler: re-enables interrupts on exit (Sony ExitHandler idiom). */
void func_002311A0(int unused0, int unused1, int value) {
    func_0025A710(value);
    __asm__ volatile ("sync\n\tei");
}
