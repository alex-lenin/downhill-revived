/* UNIT 0x00221608 0x00221888 -O0 -fno-opt-stack */
#include "animation_weight.h"
#if defined(__mips__)
#define CallShared00203B68_0 func_00203B68
#else
#define CallShared00203B68_0 Forward00203B68_0
#endif
typedef struct Sound { int words[4]; } Sound;
extern Sound *D_00279A64;
extern float D_006610C4;
extern int D_00663100, D_00663104, D_00663108, D_0066310C, D_00663110;
extern void func_00202D78(void);
extern void func_00203B68(Sound *, float, int);
extern void func_00206698(Sound *);
extern void func_00218720(int, int, int, int);
extern void func_0021F1B8(int, int, int, int);
extern void func_0021F210(int, int, int, int, int);
extern int func_0021F3F8(void);
extern void func_00238D10(void);
extern void func_0023CB00(int);
extern int func_0023D5F0(int);
extern void func_002433A8(int, int);
extern void func_0025A600(void);
extern void func_0025A700(int);
extern void func_0025A720(int);
extern void func_0025A960(int);
void func_00221608(int arg0)
{
    int handle;
    float volume;
    Sound sound;
    for (;;) {
        if (D_0066310C) break;
        func_0025A720(D_00663100);
        func_00238D10();
        handle = func_0023D5F0(5);
        func_0025A960(0);
        func_002433A8(0, 0);
        func_0021F1B8(handle, D_00663108 & 1, 0, 0);
        func_002433A8(0, 0);
        if (func_0021F3F8()) func_0021F210(5, handle, D_00663108, D_00663104, 1);
        else func_0021F210(5, handle, D_00663108, 0, 1);
        func_0023CB00(5);
        if (D_00663110) func_00218720(D_00663110, (D_00663108 ^ 1) & 1, D_00663104, 5);
        func_002433A8(0, 0);
        if (D_00279A64) {
            volume = (float)D_00663108 / D_006610C4;
            if (volume > 1.0f) volume = 1.0f;
            sound = *D_00279A64;
            CallShared00203B68_0(&sound, volume, 1);
            func_00206698(D_00279A64);
            func_00202D78();
        }
        D_00663108++;
        D_00663104 = D_00663104 == 0;
        func_0025A700(D_00663100);
        func_0025A600();
    }
}
