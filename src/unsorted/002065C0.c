/* UNIT 0x002065C0 0x00206698 */
typedef struct List {
    char p0[0x1680];
    int count;
} List;

extern int D_0029E160;
extern int D_004D0108[];
extern char D_004D0248[];
extern List D_004D0380;
extern int func_00238F60(void);
extern int func_00202C48(char *, void *);
extern void func_00202B70(char *);
extern void func_00206440(int, void *);

void func_002065C0(void) {
    int i;
    int found;
    void *buf;

    i = 0;
    D_0029E160 += func_00238F60();
    buf = __builtin_alloca(0x20);
    found = func_00202C48(D_004D0248, buf);
    func_00202B70(D_004D0248);
    for (; i < D_004D0380.count; i++) {
        if (D_004D0108[i] != 0) {
            func_00206440(D_004D0108[i], found ? buf : 0);
        }
    }
}
