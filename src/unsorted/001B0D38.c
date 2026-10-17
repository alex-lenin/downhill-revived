/* UNIT 0x001B0D38 0x001B0E00 */
#include "camera.h"
#include "rider.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef SceneNode Pose;

typedef SceneNode Info;

typedef Rider Body;

typedef RiderComponent Component;

typedef RiderCamera Obj;

extern unsigned char D_0029D3F0;

extern int func_00130010(Body *body);




extern void func_0012C7E8(Body *body);
extern void func_001B1100(void *p, Component *rider);

void func_001B0D38(Obj *obj)
{
    Body *body = obj->xA10->rider;

    if (body->x7984 != 0 || obj->xA10->node->x5 != 0 || D_0029D3F0 != 0
        || func_00130010(body) != 0) {
        func_001B0EA8(obj);
        func_001B33A8(obj);
        func_001B3E98(obj, (u128 *)&obj->xA10->node->position);
        func_001AEB88((RiderCameraRig *)((char *)body + 0x46D0), (RiderCamera *)(obj));
        func_0012C7E8(body);
        func_001B1100(obj->x5C0, obj->xA10);
    } else {
        /* quadword vector copy (inline asm, as in libvu0) */
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0"
                          : "=m"(obj->xA10->node->position) : "r"(&body->bodyNode->position) : "$2");
    }
}
