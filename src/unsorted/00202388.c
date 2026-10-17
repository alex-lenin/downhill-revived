/* UNIT 0x00202388 0x002024D0 -fno-opt-stack */
/* 989snd double-buffered RPC command queue, indexed by the current buffer D_00279468. */
extern int *D_00279448[];  /* buffer: word 0 = command count */
extern int D_00279450[];   /* bytes still free in each buffer */
extern void *D_00279460[];
/* A plain (non-array) int that is not small data: the scalar lets the scheduler see the
   store to it does not alias the buffer array accesses (fixes the tail's order), and the
   section keeps it %hi/%lo-addressed like retail. */
extern int D_00279468 __attribute__((section(".data")));
extern int D_002794A0[];
extern char D_00297860[]; /* "989snd.c: RPC collision!\n" */
extern char D_004C8940[];

extern void func_002016E8(void *, int, int *);
extern int sceSifCheckStatRpc(void *);
extern void func_00254D08(char *, ...);
extern void func_0025A960(int);
extern int func_0025CF88(void *, int, int, void *, int, void *, int, void *, void *);

void func_00202388(void)
{
    int *send;
    int index;

    func_002016E8(D_00279460[D_00279468], *D_00279448[D_00279468], D_00279448[D_00279468]);
    while (sceSifCheckStatRpc(D_004C8940)) {
        if (D_002794A0[0] == 0) {
            func_00254D08(D_00297860);
        }
        func_0025A960(0);
    }
    index = D_00279468;
    send = D_00279448[index];
    func_0025CF88(D_004C8940, 0x4D, 1, send, 0x1000 - D_00279450[index],
                  D_00279460[index], *send * 4 + 8, 0, 0);
    index = D_00279468 != 1;
    D_00279468 = index;
    *D_00279448[index] = 0;
    D_00279450[index] = 0xFFC;
}
