/* UNIT 0x00201718 0x002018C8 -fno-opt-stack */
extern int D_00279430[];
extern int D_00279470[];
extern int D_002794A0[];
extern unsigned int D_004CD400[];
extern int D_004CD440[];
extern char D_004CD3C0[];
extern char D_00297860[], D_002978E8[], D_00297918[], D_00297938[];

extern int func_00202820(int);
extern int sceSifCheckStatRpc(void *);
extern void func_00254D08(void *);
extern void func_002013F8(void);
extern void func_0025A960(int);
extern int func_0025CF88(void *, int, int, void *, int, void *, int, void *, void *);

unsigned int func_00201718(int arg0, int arg1)
{
    D_00279430[0] = 0;
    if (D_00279470[0]) {
        if (!D_002794A0[0])
            func_00254D08(D_002978E8);
        return 0;
    }
    if (func_00202820(1) == 1) {
        if (!D_002794A0[0])
            func_00254D08(D_00297918);
        return 0;
    }
    D_004CD400[0] = 0xFFFFFFFF;
    D_004CD440[1] = arg1;
    D_004CD440[0] = arg0;
    while (sceSifCheckStatRpc(D_004CD3C0)) {
        if (!D_002794A0[0])
            func_00254D08(D_00297860);
        func_002013F8();
        func_0025A960(0);
    }
    if (func_0025CF88(D_004CD3C0, 3, 1, D_004CD440, 8, D_004CD400, 4, 0, 0) < 0) {
        if (!D_002794A0[0])
            func_00254D08(D_00297938);
        D_00279430[0] = 0x106;
        return 0;
    }
    /* the RPC reply overwrites the 0xFFFFFFFF marker */
    while (D_004CD400[0] == 0xFFFFFFFF)
        func_0025A960(0);
    return D_004CD400[0];
}
