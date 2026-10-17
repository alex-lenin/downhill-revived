/* UNIT 0x001F8CA8 0x001F8DD8 */
typedef struct {
    int x0;
    char pad4[0x414];
    int x418;
    char pad41c[0x84];
    int x4a0;
    int x4a4;
    char pad4a8[0x58];
    char x500[0x64];
    char x564[16];
} Obj;

extern int func_001F8458(int);
extern int func_001F8440(int);
extern int func_00193F68(void *, int);
extern void func_0018E408(void *, int, int);
extern void func_00203590(void *, int, int, int);
extern void func_002038E0(void *, int);
extern void func_001AC7B0(void *, void *);
extern void func_001AC810(void *, float);
extern int D_004B26AC[];
extern char D_003F53FD[];
extern char D_004225A0[];
extern int *D_0029E050;

void func_001F8CA8(Obj *p)
{
    int i;
    int tmp[3];

    p->x0 = 0;
    if (!func_001F8458(D_004B26AC[0])) {
        D_003F53FD[0] = 1;
        return;
    }
    p->x4a0 = 2;
    p->x4a4 = 3;
    p->x418 = 0;
    for (i = 0; func_001F8440(i) >= 0; i++) {
        func_0018E408(p, D_0029E050[i], func_00193F68(D_004225A0, func_001F8440(i)));
    }
    tmp[2] = 0;
    func_00203590(tmp, 0x25B1, 0, 2);
    func_002038E0(tmp, 1);
    func_00203590(p->x564, 0x25B1, 0, 1);
    func_001AC7B0(p->x500, p->x564);
    func_001AC810(p->x500, 0.0f);
}
