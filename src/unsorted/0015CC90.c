/* UNIT 0x0015CC90 0x0015CDD8 */
struct Obj { char pad[0x45c]; int x45c; };
struct Field { int x0; };
struct Menu { char pad[0x64]; struct Field x64; char pad68[0xc5]; unsigned char x12d; };
struct Selection { char pad[0x2a0]; int x2a0; char pad2a4[0xb8]; unsigned char x35c; };
extern char D_004225A0[];
extern char D_003F52D0[];
extern struct Selection D_004B25A0;
extern int D_0026EE50[];
extern int func_00193F38(void *);
extern int func_00190D60(void *, int, int);
extern int func_001F8440(int);
extern void func_001BA3B8(void *, int);
void func_0015CC90(struct Obj *obj, int wanted) {
    char *menu;
    int selected = 0;
    int count = 0;
    unsigned int mask = func_00193F38(D_004225A0);
    int index;
    menu = D_003F52D0;
    if (!func_00190D60(menu + 0x64, 8, obj->x45c)) {
        for (index = 0; func_001F8440(index) >= 0; index++) {
            if (((1 << func_001F8440(index)) & mask) && func_001F8440(index) != 21 && func_001F8440(index) != 23) {
                if (count == wanted) selected = func_001F8440(index);
                count++;
            }
        }
        D_004B25A0.x2a0 = selected;
        func_001BA3B8(&D_004B25A0, D_0026EE50[selected]);
        D_003F52D0[0x12d] = 1;
        D_004B25A0.x35c = 0;
    }
}
