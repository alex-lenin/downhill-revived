/* UNIT 0x001DBBB8 0x001DBC10 */
#include "disc_path.h"
extern char D_004B25A0[];
extern long D_0029D630;
extern char D_0029D628[];

extern void func_00163A30(void *, void *);
void func_001DBBB8(void *object) {
    char buffer[32];
    char *value = GetResourceNameLegacy(D_004B25A0);
    BuildDiscPathWithMode((char *)(buffer), (const char *)(&D_0029D630), (const char *)(value), (const char *)(D_0029D628), 0);
    func_00163A30(object, buffer);
}



