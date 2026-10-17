/* UNIT 0x0013F3F8 0x0013F4D8 */
struct Global {
    char pad0[0x5BEC];
    int x5BEC;
    int x5BF0;
    int x5BF4;
};
struct Menu {
    char pad0[0x418];
    int count;
    char pad41C[0x600 - 0x41C];
    int ids[0x40];
    signed char values[0x40];
};

extern struct Global D_00398C40;
extern char D_003F52D0[];
extern void *func_0013AF60(int);
extern void func_00218498(void *, int);
extern void func_001B5AF0(void *, int, int);

void func_0013F3F8(struct Menu *menu)
{
    int i;
    int count;

    func_00218498(func_0013AF60(0x25B2), 0);
    func_00218498(func_0013AF60(0x25B3), 0);
    count = menu->count;
    for (i = 0; i < count; i++) {
        if (menu->ids[i] == 0x3B) D_00398C40.x5BF4 = menu->values[i] != 0;
        else if (menu->ids[i] == 0x37) D_00398C40.x5BF0 = menu->values[i] != 0;
        else if (menu->ids[i] == 0x42) D_00398C40.x5BEC = menu->values[i] != 0;
    }
    func_001B5AF0(D_003F52D0, 1, 0);
}
