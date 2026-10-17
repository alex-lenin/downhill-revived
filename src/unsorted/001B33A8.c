/* UNIT 0x001B33A8 0x001B3440 */
#include "camera.h"
#include "rider.h"




typedef RiderCamera Obj;

extern void func_001B3D78(Obj *obj, void *a, void *b);
extern void func_001B3E08(Obj *obj, void *a, void *b);
extern void func_001B3440(Obj *obj);
extern void func_001B3AE0(Obj *obj);
extern void func_001B3BE0(Obj *obj);



void func_001B33A8(Obj *obj)
{
    Rider *game = obj->xC0->rider;
    char *base = (char *)game->bodyNode;
    int index = game->riderIndex;
    char *a = base + 0x40;
    char *b = base + 0x10;

    func_001B3D78(obj, a, b);
    func_001B3E08(obj, a, b);
    func_001B3440(obj);
    func_001B3AE0(obj);
    func_001B3BE0(obj);
    func_001B3858(obj);
    func_0023AEB8(index);
}
