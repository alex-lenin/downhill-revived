/* UNIT 0x00221438 0x002214B0 -O0 -fno-opt-stack */
extern int D_006610F8, D_0066310C;
extern void func_0025A540(int), func_0025A5C0(int);
int func_00221438(int arg0) {
    if (D_0066310C) func_0025A540(D_006610F8);
    else func_0025A5C0(D_006610F8);
    __asm__ volatile ("sync\n\tei");
    return 0;
}
