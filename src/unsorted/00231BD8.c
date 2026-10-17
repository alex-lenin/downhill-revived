/* UNIT 0x00231BD8 0x00231C10 */
extern void func_00232950(void *);
extern void func_00232A38(void *);
void func_00231BD8(void *p) {
    __asm__ volatile ("sqc2 $vf0, 0x60(%0)" : : "r"(p) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x70(%0)" : : "r"(p) : "memory");
    func_00232950(p);
    func_00232A38(p);
}
