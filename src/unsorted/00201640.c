/* UNIT 0x00201640 0x002016E8 -fno-opt-stack */
extern unsigned int *D_00279428[];
extern int D_0027942C[];
extern int D_002794A0[];
extern char D_002977D8[];
extern char D_004C8940[];
extern void func_0025A960(int);
extern int sceSifCheckStatRpc(void *);
extern void func_00254D08(void *);

int func_00201640(void)
{
    unsigned int value;

    func_0025A960(0);
    if (!D_00279428[0]) {
        return 1;
    }
    if (!sceSifCheckStatRpc(D_004C8940)) {
        value = *D_00279428[0];
        if (value == 0xFFFFFFFF && D_00279428[0][D_0027942C[0] + 1] == value) {
            D_00279428[0] = 0;
            return 1;
        }
        if (!D_002794A0[0]) {
            func_00254D08(D_002977D8);
        }
        return 0;
    }
    return 0;
}
