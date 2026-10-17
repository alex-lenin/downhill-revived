/* UNIT 0x00198AC0 0x00198BF8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x2A0 - 0x28C];
    int x2A0;
    char pad2A4[0x374 - 0x2A4];
    char x374[0x4F0 - 0x374];
    char x4F0[1];
} State;

typedef struct Global {
    char pad0[0x12D];
    char x12D;
} Global;

extern char D_004225A0[];
extern State D_004B25A0;
extern Global D_003F52D0;
extern int D_0026EE50[];
extern int func_00193F38(void *);
extern int func_001F8440(int);
extern void func_001BA3B8(State *, int);
extern void func_001C5B80(void *);
extern void func_001C57A8(void *, u128);

void func_00198AC0(void *arg0, int index)
{
    int n;
    int i;
    int sel;
    unsigned int mask;

    n = 0;
    i = 0;
    sel = 0;
    mask = func_00193F38(D_004225A0);
    for (; func_001F8440(i) >= 0; i++) {
        if ((1 << func_001F8440(i)) & mask) {
            if (n == index) {
                sel = func_001F8440(i);
            }
            n++;
        }
    }
    D_004B25A0.x288 = 1;
    D_004B25A0.x2A0 = sel;
    func_001BA3B8(&D_004B25A0, D_0026EE50[sel]);
    D_004B25A0.x10C = 14;
    func_001C5B80(D_004B25A0.x374);
    func_001C57A8(D_004B25A0.x4F0, ((u128)0x454D4147 << 64) | (u128)0x5F5245544E455F53LL);
    D_003F52D0.x12D = 1;
}
