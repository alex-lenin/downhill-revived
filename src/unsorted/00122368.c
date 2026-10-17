/* UNIT 0x00122368 0x001223C0 */
#include "disc_path.h"
extern char D_0029C1A0[];
extern int D_0029C1A8;
extern char D_004B25A0[];

extern void func_00163A30(void *, void *);
void func_00122368(void *obj) {
    char value[32];
    char *result = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(value), (const char *)(&D_0029C1A8), (const char *)(result), (const char *)(D_0029C1A0), 0);
    func_00163A30(obj, value);
}

