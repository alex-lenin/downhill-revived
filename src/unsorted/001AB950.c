/* UNIT 0x001AB950 0x001AB9A8 */
#include "disc_path.h"
extern char D_004B25A0[];
extern long D_0029D3D8;
extern char D_0029D3D0[];

extern void func_001ABA48(void *, void *);
void func_001AB950(void *object) {
    char buffer[32];
    char *value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029D3D8), (const char *)(value), (const char *)(D_0029D3D0), 0);
    func_001ABA48(object, buffer);
}


