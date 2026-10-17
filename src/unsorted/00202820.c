/* UNIT 0x00202820 0x002028C8 -fno-opt-stack */
extern int D_00279434[];
extern int D_00279440[];
extern volatile int D_004CD380[];

extern int func_002474E0(void);
extern void func_0025A960(int);
extern int func_002013F8(void);

int func_00202820(int wait)
{
    if (!D_00279434[0])
        return func_002474E0();
    func_0025A960(0);
    D_00279440[0] = D_004CD380[0] == 0;
    if (D_00279440[0] == 1)
        return 0;
    if (wait == 1)
        return 1;
    while (D_00279440[0] == 0) {
        func_002013F8();
        func_0025A960(0);
        D_00279440[0] = D_004CD380[0] == 0;
    }
    return 0;
}
