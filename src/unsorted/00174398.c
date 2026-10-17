/* UNIT 0x00174398 0x001743F0 */
#include "disc_path.h"
extern char D_004B25A0[];
extern char D_0029C8E8[]; extern int D_0029C900;

extern void func_00163A30(void *, void *);
void func_00174398(void *obj) {
    char temp[32]; char *value;
    value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(temp), (const char *)(&D_0029C900), (const char *)(value), (const char *)(D_0029C8E8), 0);
    func_00163A30(obj, temp);
}




