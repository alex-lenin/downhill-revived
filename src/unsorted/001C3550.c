/* UNIT 0x001C3550 0x001C3638 */
#include "sound.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    int x0;
    int x4;
    int x8;
    char padC[0x20 - 0xC];
    int x20[1];
} Obj;

extern int D_004BD8B4[];

extern int func_0023CF78(int);
extern char *func_0023CF58(int);
extern int func_001C3638(Obj *obj);
extern float func_00222640(char *p);
#include "sound_math.h"
extern int func_00208E40(int a, int b, int c, int d);

void func_001C3550(Obj *obj, u128 name)
{
    int a;
    char *b;
    int idx;
    int c;
    int count;

    a = func_0023CF78(0);
    b = func_0023CF58(0) + 0x10;
    idx = func_001C3638(obj);
    if (idx == -1)
        return;
    c = func_00208C60((float *)(a), (float *)(obj->x0), func_00222640(b));
    count = func_00208E40(a, obj->x0, 0xD, 0x11);
    if (count > 0) {
        func_00201BD0(D_004BD8B4[0], 0, name, count, c, 0, 0, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)((long)&obj->x20[idx]));
        obj->x8 = 0;
    }
}
