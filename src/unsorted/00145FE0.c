/* UNIT 0x00145FE0 0x00146038 */
#include "disc_path.h"
extern char D_0029C558[], D_004B25A0[];
extern int D_0029C560;

extern void func_00163A30(void *, void *);
void func_00145FE0(void *obj) {
    char buffer[0x20];
    char *value;
    value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029C560), (const char *)(value), (const char *)(D_0029C558), 0);
    func_00163A30(obj, buffer);
}



