/* UNIT 0x001413D0 0x001414A0 */
#include "disc_path.h"
typedef struct Obj {
    char pad0[0xA4];
    int count;
    char padA8[0x130 - 0xA8];
    char *x130;
} Obj;

extern char D_0029C538[]; /* "BIKE" */
extern char D_0029C540[]; /* "R" */


extern void func_0020D4B0(void *, char *, void *, int);

void func_001413D0(Obj *obj, void *arg1, void *arg2, int *names, void *arg4)
{
    int i;

    for (i = 0; i < obj->count; i++) {
        BuildDiscPathWithMode((char *)(arg2), (const char *)(i ? D_0029C538 : D_0029C540), (const char *)(names[i]), (const char *)(arg4), 1);
        func_0020D4B0(arg1, obj->x130 + i, arg2, 0);
    }
}
