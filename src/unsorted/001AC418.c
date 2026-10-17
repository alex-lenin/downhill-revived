/* UNIT 0x001AC418 0x001AC578 */
#include "camera.h"
#include "rider.h"

typedef AnimationHandle Result;

typedef struct Sound {
    int *x0;
    int x4;
} Sound;


extern void func_00254D08(char *msg, int val);
extern void func_001AC620(Sound *obj, Result *src);
extern void func_001AC680(Sound *obj, float value);
extern void func_00202E00(void *obj);

extern void func_001634C8(void *obj);
extern char D_0028E900[];

void func_001AC418(RiderComponent *obj)
{
    int i;
    Result *ptr;
    int base;
    void *p1054;
    Result res;
    Rider *r;
    int *pp;
    int result;
    long active;
    Result *src;

    r = obj->rider;
    *(int *)((char *)r + 0x41F0) = 0;
    r = (Rider *)((char *)r + 0x41F0);
    ((int *)r)[1] = 0;
    ((int *)r)[2] = 0;
    if (obj->rider == 0) {
        return;
    }
    base = obj->rider->x796C + 16;
    res.x8 = 0;
    func_002037D0(&res, base, 0, obj->x2164);
    if (res.x0 == 0) {
        func_00254D08(D_0028E900, base);
    }
    i = 1;
    p1054 = obj->x1054;
    ptr = &obj->handles[1];
    do {
        func_002036C8(ptr, base, 0, i, obj->x2164);
        i++;
        ptr++;
    } while (i < 0xFE);
    if (obj->rider->x796C == 0x200) {
        /* Materialize &handles[252] here: retail keeps it in $a1 across the
           null check so that it lands in the check's delay slot. */
        src = &obj->handles[252];
        if (obj->handles[252].x0 != 0) {
            func_001AC620((Sound *)obj->xFF4, src);
        pp = ((Sound *)obj->xFF4)->x0;
        active = 0;
        if (pp != 0) {
            if (pp[0] != 0) {
                result = pp[1] != 0;
                active = result;
            }
        }
        if ((active & 0xFF) != 0) {
            func_001AC680((Sound *)obj->xFF4, 0.0f);
        }
        }
    }
    func_00202E00(obj->handles[3].x0);
    if (obj->x2160) {
        func_001B0C10((RiderCamera *)((char *)obj + 0x16F0), (struct RiderComponent *)(obj));
    }
    func_001634C8(p1054);
}
