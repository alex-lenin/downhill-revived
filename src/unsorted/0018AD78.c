/* UNIT 0x0018AD78 0x0018ADC8 */
#include "menu_refresh.h"
extern int D_0029E4C0;
extern char D_003EB760[];
extern char D_003F52D0[];
extern void func_00184938(void *, int);

extern void func_00190740(void *, int);
void func_0018AD78(void *obj)
{
    func_00184938(D_003EB760, 0);
    RefreshGlobalMenuLegacy(obj);
    func_00190740(D_003F52D0, D_0029E4C0);
}
