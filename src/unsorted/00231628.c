/* UNIT 0x00231628 0x00231798 */
extern int D_0029E344;
extern int D_0029E358;
extern int D_0029E35C;
extern int D_0029E380;
extern int D_0029E384;
extern int D_0029E348[2];
extern int D_0029E370[2];
extern int D_00738E40[];
extern int D_00738F30[];
extern int D_0073A178[];
extern void (*D_0029E368)(int, int, int);
extern void func_00242118(int, int);
extern int func_0025A600(void);
extern void func_0025A700(int);
extern int func_0025A720(int);

/* D_0029E348/D_0029E370 are two-entry (double-buffer) arrays declared with their size so
   the compiler emits `la` for them, as retail does. */
void func_00231628(void)
{
    D_0029E358 = 0;
    D_0029E384 = 0;
    func_0025A600();
    for (; D_0029E384 < D_0029E35C; D_0029E384++) {
        D_0029E380 = 0;
        func_0025A720(D_0029E348[D_0029E358]);
        if (D_00738F30[D_0029E384] != 0) {
            func_00242118(D_00738E40[D_0029E384], D_0029E370[D_0029E358]);
        }
        if (D_0029E368 != 0) {
            D_0029E368(D_00738E40[D_0029E384], 0x54840, D_0073A178[D_0029E384]);
        }
        func_0025A700(D_0029E348[D_0029E358]);
        D_0029E358 = !D_0029E358;
    }
    D_0029E344 = 1;
}
