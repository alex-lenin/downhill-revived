/* UNIT 0x0019B780 0x0019B8E8 */
#include "menu_finalize.h"
typedef struct {
    int x0;
    char pad4[0x418 - 0x4];
    int x418;
    char pad41C[0x460 - 0x41C];
    int x460;
    char pad464[0x478 - 0x464];
    int x478;
    char pad47C[0x4A8 - 0x47C];
    int x4A8;
} Menu;


typedef struct {
    char pad0[0x12195];
    unsigned char x12195;
    unsigned char x12196;
    unsigned char x12197;
} Save;

extern int D_0026F7F8[];
extern char D_0029DE68[];
extern Save D_004225A0;
extern char D_0043CD50[];
extern int D_0043D7A8[];
extern signed char D_0043DCC0[];

extern void func_0019BDA0(signed char *, int, int);
extern void func_0018E638(Menu *, int, char *);
extern int func_0019B0B8(char *, int);
extern void func_0018E408(Menu *, int, int);

extern void func_0018E6B8(Menu *, int, char *);

void func_0019B780(Menu *menu)
{
    int i;

    menu->x460 = 40;
    menu->x0 = 0;
    func_0019BDA0(D_0043DCC0, 25, D_004225A0.x12195);
    func_0019BDA0(D_0043DCC0, 30, D_004225A0.x12196);
    func_0019BDA0(D_0043DCC0, 35, D_004225A0.x12197);
    if (D_0043D7A8[0] == 0) {
        func_0018E638(menu, 0x86, D_0029DE68);
    } else {
        func_0018E638(menu, 0x87, D_0029DE68);
    }
    menu->x418 = 0;
    for (i = 0; i < 40; i++) {
        if (func_0019B0B8(D_0043CD50, i) == 0 && D_0043DCC0[i + 0x24] != 0) {
            func_0018E408(menu, D_0026F7F8[i], 1);
        }
    }
    menu->x478 = 1;
    FinalizeMenuLegacy(menu);
    func_0018E6B8(menu, 0xA9, D_0029DE68);
    menu->x4A8 = 5;
}
