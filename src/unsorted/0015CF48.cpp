/* UNIT 0x0015CF48 0x0015D080 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Slot { char pad[12]; short id; char padE[2]; bool Is(int value) { return id == value; } };
struct Menu { char pad[16]; Slot slots[72]; u128 x490; void func_0015C920(); };
extern char D_004B2A90[], D_003F52D0[], D_00386680[], D_00387A90[], D_0038B1F0[], D_0038BC00[];
extern char *D_0038C600[];
extern char D_003F53FD[];
extern void func_001C57A8(char *, u128);
extern void func_0015C990();
extern void func_00190660(char *, char *);
extern void func_00190890(char *);
extern void func_00187AB0();
void func_0015CF48(Menu *menu, int index) {
    if (menu->slots[index].Is(0x1D)) {
        menu->func_0015C920();
        D_0038C600[0] = D_0038BC00;
        D_003F53FD[0] = 1;
    } else if (menu->slots[index].Is(0x28)) {
        menu->x490 = 0;
        func_001C57A8(D_004B2A90, (u128)0x5443454C45535F53UL);
        func_0015C990();
    } else if (menu->slots[index].Is(0x29)) {
        func_00190660(D_003F52D0, D_00386680);
    } else if (menu->slots[index].Is(0x30)) {
        func_00190660(D_003F52D0, D_0038B1F0);
    } else if (menu->slots[index].Is(0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else if (menu->slots[index].Is(0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    }
}
