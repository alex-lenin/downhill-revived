/* UNIT 0x001DC8D8 0x001DC910 */
typedef struct { char pad0[0x30]; int x30, x34, x38, x3C, x40, x44, x48, x4C, x50, x54; } Obj;
void func_001DC8D8(Obj *p) {
    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(p) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x10(%0)" : : "r"(p) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x20(%0)" : : "r"(p) : "memory");
    p->x30 = 0;
    p->x34 = 0;
    p->x38 = 0;
    p->x3C = 0;
    p->x40 = 0;
    p->x44 = 0;
    p->x48 = 0;
    p->x4C = 0;
    p->x50 = 0;
    p->x54 = 0;
}
