/* UNIT 0x00118380 0x001183E0 */
#include "disc_path.h"
extern char D_004B25A0[];
extern char D_0029C118[];
extern char D_0029C130[8];


extern void func_00117658(void *, char *);
extern void func_001183E0(void *);

void func_00118380(void *p)
{
    char path[32];
    char *name;

    name = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(path), (const char *)(D_0029C130), (const char *)(name), (const char *)(D_0029C118), 0);
    func_00117658(p, path);
    func_001183E0(p);
}
