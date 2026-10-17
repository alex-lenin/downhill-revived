/* UNIT 0x00144720 0x00144778 */
#include "disc_path.h"
extern char D_0029C548[], D_004B25A0[];
extern int D_0029C550;

extern void func_00163A30(void *, void *);
void func_00144720(void *obj) {
    char buffer[0x20];
    char *value;
    value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029C550), (const char *)(value), (const char *)(D_0029C548), 0);
    func_00163A30(obj, buffer);
}


