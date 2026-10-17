/* UNIT 0x001F8E90 0x001F8F50 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Global {
    char pad0[0x2A0];
    int x2A0;
    char pad2A4[0x374 - 0x2A4];
    char x374[0x4F0 - 0x374];
    char x4F0[1];
} Global;

typedef struct Arg {
    int x0;
} Arg;

extern Global D_004B25A0;
extern char D_00398C40[];
extern int D_0026EE50[];
extern char D_003F53FD[];

extern int func_001F8440(int);
extern void func_001BA3B8(Global *, int);
extern void func_0016C2A0(void *, int);
extern void func_001C5B60(void *);
extern void func_001C59B0(void *);
extern void func_001C5958(void *);
extern void func_001C57A8(void *, u128);

#define NAME_S_ENTER_GAME (((u128)0x454D4147LL << 64) | (u128)0x5F5245544E455F53LL)

void func_001F8E90(Arg *arg)
{
    int mode = func_001F8440(arg->x0);

    D_004B25A0.x2A0 = mode;
    func_001BA3B8(&D_004B25A0, D_0026EE50[mode]);
    func_0016C2A0(D_00398C40, 50);
    func_001C5B60(D_004B25A0.x374);
    func_001C57A8(D_004B25A0.x4F0, NAME_S_ENTER_GAME);
    func_001C59B0(D_004B25A0.x4F0);
    func_001C5958(D_004B25A0.x4F0);
    D_003F53FD[0] = 1;
}
