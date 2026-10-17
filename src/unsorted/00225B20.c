/* UNIT 0x00225B20 0x00225CE8 */
#include "gs_packet.h"
typedef struct Obj { char pad0[0x27C8]; float x27C8, x27CC, x27D0, x27D4; short x27D8, x27DA; char pad27DC[0x48]; int x2824; char pad2828[8]; char *x2830, *x2834, *x2838; char pad283C[0x18]; int x2854; } Obj;
typedef struct Packet { long x0; int x8, xC; unsigned int x10, x14, x18, x1C, x20; } Packet;
typedef struct Params { float x0, x4, x8, xC, x10, x14, x18, x1C, x20, x24, x28, x2C; int x30, x34, x38; } Params;


extern int func_0023DFA0(void);
extern int func_002249A8(int, int);
void func_00225B20(Obj *obj) {
    char *cursor;
#define p ((Packet *)cursor)
#define v ((Params *)cursor)
#define end cursor
    float f;
    obj->x2854 = 256;
    cursor = (char *)(Packet *)func_002246A0((unsigned int *)((void *)(((unsigned int)obj + 12) & ~15U)), obj->x2824, 2);
    p->x0 = 0x378017F801AL;
    p->x8 = 8;
    p->xC = 0;
    p->x10 = 0x80828282;
    p->x14 = 0x3F800000;
    p->x18 = 1;
    p->x1C = 0;
    p->x20 = 0x6C040108;
    cursor = (char *)(Params *)func_00224660((GsPacket *)((char *)p + 0x24), 0x53, 0x53535353, 10, 0, 0x114, 1, 0, 0);
    v->x0 = 522496.0f;
    if (func_0023DFA0()) f = 522352.5f;
    else f = 522352.0f;
    v->x4 = f;
    v->x8 = 0.8660254f;
    v->xC = -0.5f;
    v->x10 = obj->x27C8 + 1.25f;
    v->x14 = obj->x27C8 + 1.0f;
    v->x18 = obj->x27D0;
    v->x1C = obj->x27CC;
    v->x20 = obj->x27C8;
    v->x24 = obj->x27C8;
    v->x28 = -obj->x27D0;
    v->x2C = obj->x27D4 * obj->x27DA;
    v->x30 = 0;
    v->x34 = 0;
    cursor += 0x3C;
    obj->x2830 = end;
    end += func_002249A8(obj->x2824, 1)*4 + 0x1000;
    obj->x2834 = end;
    end += 0x1000;
    obj->x2838 = end;
}
