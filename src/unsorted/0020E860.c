/* UNIT 0x0020E860 0x0020E920 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct State { float x0, x4, x8, xC, x10, x14, x18; int x1C; };

extern struct State D_004F3200;
extern u128 D_004F32E0[][2];
extern float D_004F33A0[];
extern int D_004F33B8[];
extern int D_004F33D0[];

void func_0020E860(void) {
    int i;

    D_004F3200.x0 = 400.0f;
    D_004F3200.x4 = 4000.0f;
    D_004F3200.x8 = 1500.0f;
    D_004F3200.xC = 255.0f;
    D_004F3200.x10 = 60.0f;
    D_004F3200.x14 = 60.0f;
    D_004F3200.x18 = 60.0f;
    D_004F3200.x1C = 1;
    for (i = 0; i < 6; i++) {
        /* sq $zero: gcc always materializes a TImode 0 with por first */
        __asm__ volatile ("sq $0, %0" : "=m"(D_004F32E0[i][0]));
        __asm__ volatile ("sq $0, %0" : "=m"(D_004F32E0[i][1]));
        D_004F33B8[i] = 0;
        D_004F33D0[i] = 0;
        D_004F33A0[i] = 1.0f;
    }
}
