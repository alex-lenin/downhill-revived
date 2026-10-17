/* UNIT 0x00171538 0x001715B8 */
extern void func_00221F20(void *);
void func_00171538(char *obj) {
    *(int *)(obj + 0x0) = 0;
    *(int *)(obj + 0x4) = 0;
    *(int *)(obj + 0x8) = 0;
    func_00221F20(obj + 0x10);
    __asm__ volatile ("sqc2 $vf0, 0x50(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x60(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x70(%0)" : : "r"(obj) : "memory");
    *(int *)(obj + 0x80) = 0;
    *(int *)(obj + 0x84) = 0;
    *(int *)(obj + 0x90) = 0;
    *(int *)(obj + 0x94) = 0;
    *(int *)(obj + 0x1E0) = 0;
    *(int *)(obj + 0x1E4) = 0;
    *(int *)(obj + 0x1E8) = 0;
    *(int *)(obj + 0x1EC) = 0;
    *(int *)(obj + 0x1F0) = 0;
    *(int *)(obj + 0x1F4) = 0;
    *(int *)(obj + 0x1F8) = 0;
    *(int *)(obj + 0x1FC) = 0;
    *(int *)(obj + 0x200) = 0;
    *(int *)(obj + 0x204) = 0;
    obj[0x208] = 0;
    obj[0x209] = 0;
}
