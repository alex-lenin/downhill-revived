/* UNIT 0x0015D180 0x0015D240 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Slot {
    char pad0[0xC];
    short id;
    char padE[2];

    bool Is(int value) { return id == value; }
};

struct Menu {
    char pad0[0x10];
    Slot slots[0x48];
    u128 x490;
};

extern char D_004B2A90[];
extern char D_003F52D0[];
extern char D_00386680[];
extern char D_00387A90[];

extern void func_001C57A8(char *, u128);
extern void func_0015C990(void);
extern void func_00190660(char *, char *);

void func_0015D180(Menu *menu, int index)
{


    if (menu->slots[index].Is(0x28)) {
        menu->x490 = 0;
        func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53UL); /* "S_SELECT" */
        func_0015C990();
    } else if (menu->slots[index].Is(0x29)) {
        func_00190660(D_003F52D0, D_00386680);
    } else if (menu->slots[index].Is(0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    }
}
