/* UNIT 0x001E1C70 0x001E1D50 */
#include "disc_path.h"
struct Header {
    char data[0x18];
};

struct ReplayFile {
    struct Header header;
    char body[0x4000 - 0x18];
};

struct Obj {
    char pad0[0x18];
    struct Header x18;
};

extern char D_004B25A0[];
extern char D_0032FC60[];
extern char D_0029D6F0[]; /* "%s" */
extern char D_0029D6F8[]; /* "orb" */
extern char D_0029D700[4]; /* "rep" */
extern int func_00254D68(char *, char *, ...);

extern int func_0020D930(void *, char *);
extern void *memcpy(void *, const void *, unsigned int);

void func_001E1C70(struct Obj *obj)
{
    struct ReplayFile file;
    char path[0x20];
    char name[0x10];
    int size;
    char *dst;

    func_00254D68(name, D_0029D6F0, GetResourceNameLegacy(D_004B25A0));
    BuildDiscPathWithMode((char *)(path), (const char *)(D_0029D700), (const char *)(name), (const char *)(D_0029D6F8), 0);
    size = func_0020D930(&file, path);
    if (size > 0) {
        obj->x18 = file.header;
        dst = D_0032FC60;
        dst += 0x2EB00;
        memcpy(dst, file.body, size - 0x18);
    }
}
