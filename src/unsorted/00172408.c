/* UNIT 0x00172408 0x00172460 */
#include "disc_path.h"
extern char D_004B25A0[];
extern char D_0029C8D0[]; extern int D_0029C8D8;

extern void func_00163A30(void *, void *);
void func_00172408(void *obj) {
    char temp[32]; char *value;
    value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(temp), (const char *)(&D_0029C8D8), (const char *)(value), (const char *)(D_0029C8D0), 0);
    func_00163A30(obj, temp);
}



