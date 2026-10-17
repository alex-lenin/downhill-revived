/* UNIT 0x002024D0 0x00202590 -fno-opt-stack */
extern int D_00279434[];
extern int D_00279470[];
extern int func_002013F8(void);
extern int func_00202820(int);
extern int func_00201E90(int, int, int *);

int func_002024D0(int a, int b, int c, int d) {
    int args[4];

    if (D_00279434[0] == 1) {
        return 0;
    }
    if (D_00279470[0]) {
        while (func_002013F8()) {
        }
    }
    func_00202820(0);
    args[0] = a;
    args[1] = b;
    args[2] = c;
    args[3] = d;
    return D_00279434[0] = func_00201E90(0x2A, 0x10, args);
}
