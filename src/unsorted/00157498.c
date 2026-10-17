/* UNIT 0x00157498 0x00157590 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackAddressDiscard
#endif
#include "ground_probe.h"
#if defined(__mips__)
#define CallGroundProbe func_00211980
#else
#define CallGroundProbe ProbeGroundLegacy
#endif
typedef struct Rider {
    char pad0[0x7A68];
    unsigned char x7A68;
} Rider;

typedef struct Owner {
    Rider *rider;
} Owner;

typedef struct Body {
    float x0, x4, x8, xC;
} Body;

typedef struct Global {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
} Global;

extern char D_0029EA20[];
extern Global D_004B25A0;
#if defined(__mips__)
extern float func_00211980(Body *body, float *out, int arg, float range);
#endif
#if defined(__mips__)
extern void func_00119180(void *a, Body *b, Body *c, int d, int flag);
#endif
extern void func_00157590(Owner *owner, Body *body, int arg);

int func_00157498(Owner *owner, Body *body, int arg)
{
    float buf[4];
    float dist;
    int flag;

    dist = CallGroundProbe(body, buf, 0, 7.0f);
    if (dist != 0.0f && dist < 40.0f) {
        body->x8 -= dist;
    }
    flag = 0;
    if (owner->rider != 0) {
        flag = owner->rider->x7A68 != 0;
    }
    CallTrackProjection(D_0029EA20, body, body, arg, flag);
    if (D_004B25A0.x10C == 0x10 || D_004B25A0.x2A0 == 0x15) {
        func_00157590(owner, body, arg);
    }
    return 1;
}
