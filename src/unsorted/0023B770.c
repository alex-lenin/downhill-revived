/* UNIT 0x0023B770 0x0023B880 */
#define REG(a) (*(volatile unsigned int *)(a))

typedef struct Obj {
    char pad0[0x1C];
    unsigned int x1C; /* D4 (toIPU) MADR */
    unsigned int x20; /* D4 TADR */
    unsigned int x24; /* D4 QWC */
    unsigned int x28; /* D4 CHCR */
    unsigned int x2C; /* D3 (fromIPU) MADR */
    unsigned int x30; /* D3 QWC */
    unsigned int x34; /* D3 CHCR */
    unsigned int x38; /* IPU_BP */
    unsigned int x3C; /* IPU_CTRL */
    int x40;
    int x44;
} Obj;

extern void func_0025A720(int);
extern void func_0025A700(int);
extern void func_0023B1B0(int);
extern void func_0023B140(int);

int func_0023B770(Obj *obj)
{
    func_0025A720(obj->x40);
    obj->x44 = 0;
    func_0023B1B0(5);
    obj->x1C = REG(0x1000B410);
    obj->x20 = REG(0x1000B430);
    obj->x24 = REG(0x1000B420);
    obj->x28 = REG(0x1000B400);
    if (REG(0x10002010) & 0xF0) {
        while (REG(0x10002010) & 0xF0) {
        }
    }
    func_0023B140(0);
    obj->x2C = REG(0x1000B010);
    obj->x30 = REG(0x1000B020);
    obj->x34 = REG(0x1000B000);
    obj->x38 = REG(0x10002020);
    obj->x3C = REG(0x10002010);
    func_0025A700(obj->x40);
    return 1;
}
