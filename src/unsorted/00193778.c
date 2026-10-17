/* UNIT 0x00193778 0x001937E0 */
#include "disc_path.h"
extern char D_00308D40[],D_004B25A0[];
extern int D_0029D090;
extern char D_0029D088[];
extern void func_001274F0(void *);

extern void func_00163A30(void *,void *);
void func_00193778(void *obj) {
 char temp[0x20];
 char *value;
 func_001274F0(D_00308D40);
 value=GetResourceNameLegacy(D_004B25A0);
 BuildDiscPathWithMode((char *)(temp), (const char *)(&D_0029D090), (const char *)(value), (const char *)(&D_0029D088), 0);
 func_00163A30(obj,temp);
}
