/* UNIT 0x001BC290 0x001BC480 */
typedef struct Block { char pad0[0x10]; int x10; int x14; } Block;

extern void func_001C0DE0(void *, int, int);
extern void func_001C1280(void *);
extern void func_001FD3B8(void *);
extern void func_00235488(void *, int, int, int);
extern void func_00247A40(int);
extern void func_002350D0(void *, void *);
extern void func_00254D68(void *, void *);
extern void func_00235790(void *, void *);
extern void func_00254D08(void *, ...);
extern int func_002472B8(void *, void *);
extern int func_00233428(void *, void *);
extern int func_00238F48(void);
extern void *func_001C2138(void *, void *, int, int);
/* returns a value that is ignored here; declaring it int is needed for the register choice */
extern int func_00233488(void *, void *, void *);
extern char D_0028F0B0[], D_0028F0C0[], D_0028F0D8[], D_0028F0F8[], D_0028F110[], D_0028F128[];
extern void *D_0029DE80, *D_0029DE84;

int func_001BC290(char *obj) {
    void *info[12];
    char name[256];
    void *out;
    char *base = obj + 4;
    Block *block;
    unsigned char ok = 0;
    int i;
    char *p;

    func_001C0DE0(obj + 0x838, 0x665, 10);
    func_001C0DE0(obj + 0x84C, 0x666, 12);
    *(int *)(obj + 0x860) = 0;
    func_00235488(base, 4, 32, 200);
    func_001C1280(obj + 0x824);
    func_00247A40(0);
    func_002350D0(base, D_0028F0B0);
    func_00254D68(name, D_0028F0C0);
    if (!func_002472B8(info, name)) {
        func_00254D08(D_0028F0D8, name);
        return 0;
    }
    func_00235790(base, info[0]);
    func_00254D08(D_0028F0F8, info[0]);
    block = func_001C2138(D_0028F0C0, 0, 96, 0);
    if (!func_00233428(base, block)) {
        if (block->x14)
            D_0029DE80 = func_001C2138(D_0028F0C0, 0, block->x14, block->x10);
        D_0029DE84 = func_001C2138(D_0028F110, &out, 0, 0);
        func_00254D08(D_0028F128, out, block->x14);
        func_00233488(base, D_0029DE84, D_0029DE80);
        if (D_0029DE84)
            ok = D_0029DE80 != 0;
    }
    if (block)
        func_001FD3B8(block);
    *(int *)(obj + 0x830) = func_00238F48();
    *(int *)(obj + 0x834) = 1;
    p = obj + 0x7D0;
    i = 3;
    do {
        *(int *)p = 0;
        i--;
        p -= 0x84;
    } while (i >= 0);
    return ok;
}
