/* UNIT 0x001F8200 0x001F8420 .bss=0x4C6530:0x4C6560 */
typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    char pad20[0x30];
    int x50;
    int x54;
    char pad58[0x3C];
    int x94;
    char pad98[0x8];
} S5DB0;

typedef struct { int x0, x4, x8, xC; } Item16;

extern S5DB0 D_004C5DB0;
extern int D_004C6360[];
extern int D_004C6370[];
extern int D_004C6380[];
extern int D_004C6390[];
extern int D_004C63A0[];
extern int D_004C63B0[];
extern int D_004C63C0[];
extern int D_004C63D0[];
extern int D_004C63E0[];
extern int D_004C63F0[];
extern int D_004C6400[];
extern int D_004C6410[];
extern int D_004C6420[];
extern int D_004C6430[];
extern int D_004C6440[];
extern int D_004C6450[];
extern int D_004C6460[];
extern int D_004C6470[];
extern int D_004C6480[];
extern int D_004C6490[];
extern int D_004C64A0[];
extern int D_004C64B0[];
extern int D_004C64C0[];
extern int D_004C64D0[];
extern int D_004C64E0[];
extern int D_004C64F0[];
extern int D_004C6500[];
extern int D_004C6510[];
extern int D_004C6520[];
extern int func_002244B8(void);
extern int func_00224498(void);
extern int func_002244A8(void);

Item16 D_004C6530 __attribute__((section(".bss"))) = { 0 };
Item16 D_004C6540 __attribute__((section(".bss"))) = { 0 };
Item16 D_004C6550 __attribute__((section(".bss"))) = { 0 };

void func_001F8200(int a, int b)
{
    if (b == 0xFFFF && a != 0) {
        D_004C5DB0.x94 = 1;
        D_004C5DB0.x18 = 0x04000000;
        D_004C5DB0.x1C = 0x04578040;
        D_004C5DB0.x8 = 0x048134C0;
        D_004C5DB0.x0 = 0x04893740;
        D_004C5DB0.x4 = 0x048ED840;
        D_004C5DB0.x50 = 0;
        D_004C5DB0.x54 = 0;
        D_004C5DB0.xC = func_002244B8();
        D_004C5DB0.x10 = func_00224498();
        D_004C5DB0.x14 = func_002244A8();
        D_004C6360[2] = 0;
        D_004C6370[2] = 0;
        D_004C6380[2] = 0;
        D_004C6390[2] = 0;
        D_004C63A0[2] = 0;
        D_004C63B0[2] = 0;
        D_004C63C0[2] = 0;
        D_004C63D0[2] = 0;
        D_004C63E0[2] = 0;
        D_004C63F0[2] = 0;
        D_004C6400[2] = 0;
        D_004C6410[2] = 0;
        D_004C6420[2] = 0;
        D_004C6430[2] = 0;
        D_004C6440[2] = 0;
        D_004C6450[2] = 0;
        D_004C6460[2] = 0;
        D_004C6470[2] = 0;
        D_004C6480[2] = 0;
        D_004C6490[2] = 0;
        D_004C64A0[2] = 0;
        D_004C64B0[2] = 0;
        D_004C64C0[2] = 0;
        D_004C64D0[2] = 0;
        D_004C64E0[2] = 0;
        D_004C64F0[2] = 0;
        D_004C6500[2] = 0;
        D_004C6510[2] = 0;
        D_004C6520[2] = 0;
        D_004C6530.x8 = 0;
        D_004C6540.x8 = 0;
        D_004C6550.x8 = 0;
    }
}
