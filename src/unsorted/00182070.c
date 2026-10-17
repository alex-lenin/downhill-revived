/* UNIT 0x00182070 0x001820C8 */
#include "disc_path.h"
struct Obj { char pad[0x208c]; int x208C; };
extern char D_004B25A0[];
extern int D_0029CAD0;

extern void func_00163A30(void *, void *);
void func_00182070(struct Obj *obj) {
    char buffer[32];
    char *value;
    value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029CAD0), (const char *)(value), (const char *)(obj->x208C), 0);
    func_00163A30(obj, buffer);
}
