/* UNIT 0x00192A50 0x00192B38 */
#include "object_registry.h"
#include "scene.h"
typedef struct Hdr {
    unsigned int x0_0 : 7;
    unsigned int x0_7 : 11;
    unsigned int x0_18 : 14;
} Hdr;

typedef SceneNode Inner;

typedef struct Elem { int x0, x4, x8, xC; } Elem;

typedef struct Obj {
    int x0;
    Inner *x4;
    void *x8;
    int xC;
    Elem elems[34];
} Obj;

extern void func_00203590(Elem *e, int a, int b, int index);
extern void func_00203CB8(Elem *e);


void func_00192A50(Obj *obj)
{
    Elem tmp;
    int i;

    for (i = 0; i < 34; i++) {
        func_00203590(&obj->elems[i], ((Hdr *)obj->x4->resource)->x0_18, ((Hdr *)obj->x4->resource)->x0_7, i);
        tmp = obj->elems[i];
        func_00203CB8(&tmp);
    }
    obj->x4->xB4 = RegisterObjectShortRetail((void *)(obj), (unsigned int)(((Hdr *)obj->x4->resource)));
    ((Hdr *)obj->x4->resource)->x0_7 = obj->x4->xB4;
}
