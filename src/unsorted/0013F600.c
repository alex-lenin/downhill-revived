/* UNIT 0x0013F600 0x0013F6F8 */
typedef struct Menu {
    char pad0[0x418];
    int count;
    char pad41C[0x500 - 0x41C];
    int x500[0x40];
    int x600[0x40];
    signed char x700[0x40];
    unsigned char x740[0x40];
    unsigned char x780[0x40];
    unsigned char x7C0[0x40];
} Menu;

typedef struct Save {
    char pad0[0x12190];
    unsigned char x12190[1];
} Save;

extern Save D_004225A0;
extern char D_0029C3D8[];

extern int func_0013F6F8(Menu *, int);
extern void func_0018E408(Menu *, char *, int);

void func_0013F600(Menu *menu, int arg1, int key, unsigned char arg3, unsigned char arg4,
                   unsigned char arg5, signed char arg6) {
    int i = menu->count;
    int index = func_0013F6F8(menu, key);
    int set = 0;

    if (index >= 0) {
        set = D_004225A0.x12190[index] != 0;
    }
    menu->x600[i] = key;
    menu->x500[i] = set ? 0 : arg1;
    menu->x700[i] = arg6;
    menu->x740[i] = arg4;
    menu->x780[i] = arg5;
    menu->x7C0[i] = arg3;
    func_0018E408(menu, D_0029C3D8, 1);
}
