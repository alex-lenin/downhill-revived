/* UNIT 0x001818C0 0x00181918 */
#include "disc_path.h"
extern char D_0029CAC0[], D_004B25A0[];
extern int D_0029CAC8;

extern void func_00163A30(void *, void *);
void func_001818C0(void *obj) {
    char buffer[32];
    char *value;
    void *name;
    value = GetResourceNameLegacy(D_004B25A0);
    name = D_0029CAC0;
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029CAC8), (const char *)(value), (const char *)(name), 0);
    func_00163A30(obj, buffer);
}



