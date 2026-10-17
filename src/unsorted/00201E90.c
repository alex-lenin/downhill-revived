/* UNIT 0x00201E90 0x00202048 -fno-opt-stack */
extern int D_00279428[];
extern int *D_00279448[];
extern int D_00279468[];
extern int D_0027946C[];
extern int D_002794A0[];
extern char D_00297860[];
extern char D_004C8940[];
extern int D_004C8980[];
extern char D_004C89C0[];

extern void func_002013F8();
extern void func_0025A960(int);
extern void func_002016E8(void *, int);
extern int sceSifCheckStatRpc(void *);
extern void func_00254D08(void *);
extern int func_0025CF88(void *, int, int, void *, int, void *, int, int, int);
extern int func_00201640(void);
extern void func_00202388(int **);

int func_00201E90(int cmd, int count, unsigned char *src) {
    int i = 0, result;
    int **p;

    for (; i < count; i++) {
        D_004C89C0[i] = src[i];
    }
    while (D_00279428[0]) {
        func_002013F8();
        func_0025A960(0);
    }
    func_002016E8(D_004C8980, 1);
    while (sceSifCheckStatRpc(D_004C8940)) {
        if (!D_002794A0[0]) {
            func_00254D08(D_00297860);
        }
        func_002013F8();
        func_0025A960(0);
    }
    if (count) {
        func_0025CF88(D_004C8940, cmd, 1, D_004C89C0, count, D_004C8980, 12, 0, 0);
    } else {
        func_0025CF88(D_004C8940, cmd, 1, 0, 0, D_004C8980, 12, 0, 0);
    }
    while (!func_00201640()) {
    }
    result = D_004C8980[1];
    p = &D_00279448[D_00279468[0]];
    if (**p && !D_0027946C[0]) {
        func_00202388(p);
    }
    return result;
}
