/* UNIT 0x001DBE38 0x001DBEF8 */
#include "timer.h"
#include "tween.h"
struct Obj {
    char *x0;
    char pad4[0x4];
    float x8;
    char padC[0x10];
    float x1C;
};

extern char D_00759780[];
extern void func_00262620(void);

extern void func_00202E38(void *, int, int, float);

extern void func_001ACF38(void *, int, float);

void func_001DBE38(struct Obj *obj)
{
    char *base;
    float one;

    base = obj->x0;
    one = 1.0f;
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00262620), obj, 1);
    func_00202E38(base + 0x2144, 1, 1, obj->x8);
    func_00202E38(base + 0x2F0, 1, 1, obj->x8);
    func_001B3170((RiderSecondaryBlend *)(base + 0x3530), (int)obj->x8, one);
    func_001ACF38(base + 0x4580, (int)(obj->x8 * 0.0625f), 0.0f);
    obj->x1C = one;
}
