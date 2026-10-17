/* UNIT 0x001DDB58 0x001DDBA0 */
struct Obj {
    int x0;
    int x4;
    int x8;
    int xC;
    char x10[0x10];
    char pad20[0x26 - 0x20];
    short x26;
    char pad28[0x30 - 0x28];
    char x30[0x10];
    char x40[0x10];
    char x50[0x10];
    char x60[0x10];
    char x70[0x10];
    float x80;
    char x84;
};

/* uses inline asm: sqc2 $vf0 stores (0,0,0,1) vectors */
void func_001DDB58(struct Obj *obj, int x0)
{
    obj->x0 = x0;
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    __asm__ volatile ("sqc2 $vf0, 0x10(%0)" : : "r"(obj) : "memory");
    obj->x26 = 0;
    __asm__ volatile ("sqc2 $vf0, 0x30(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x40(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x50(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x60(%0)" : : "r"(obj) : "memory");
    __asm__ volatile ("sqc2 $vf0, 0x70(%0)" : : "r"(obj) : "memory");
    obj->x80 = 0.33334f;
    obj->x84 = 0;
}
