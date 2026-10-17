/* UNIT 0x001990C0 0x00199118 */
#include "disc_path.h"
extern char D_004B25A0[];
extern char D_0029D0F0[];
extern int D_0029D0F8;

extern void func_001991B0(void *, void *);
void func_001990C0(void *obj) {
    char buffer[0x20];
    char *value;
    char *extension, *name;
    value = GetResourceNameLegacy(D_004B25A0);
    extension = D_0029D0F0;
    name = (char *)&D_0029D0F8;
    BuildDiscPathWithMode((char *)(buffer), (const char *)(name), (const char *)(value), (const char *)(extension), 0);
    func_001991B0(obj, buffer);
}



