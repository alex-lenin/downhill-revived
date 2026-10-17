/* UNIT 0x00172E68 0x00172FF8 */
#include "track_projection.h"
#if defined(__mips__)
#define CallTrackProjection func_00119180
#else
#define CallTrackProjection ProjectTrackDiscard
#endif
typedef struct Obj { char pad[0x874]; float x874; } Obj;
typedef struct State { char p0[8]; int x8; char *xC; Obj *x10; char *x14; char p18[0x2c]; int x44; char p48[4]; int x4C; char p50[0x40]; float x90[4]; float xA0[4]; char pB0[4]; float xB4,xB8,xBC,xC0; } State;
extern void func_00173A18(State *),func_00173358(State *),func_001733C0(State *),func_00173688(State *),func_00173560(State *);
extern int func_00222BD8(int,int);
extern float func_00222B68(float,float);
#if defined(__mips__)
extern void func_00119180(void *,void *,void *,void *,int);
#endif
extern char D_0029EA20[];
void func_00172E68(State *s) {
    float v, minimum, distance;
    func_00173A18(s);
    if (s->x44 == s->x4C) {
        if (s->xB4 < 10.0f) {
            __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(s->x90) : "r"(s->x14+16), "r"(s->xC+16) : "memory");
            if (s->x90[2] > 0.0f) {
                if (func_00222BD8(0,100) < 40) {
                    s->x8 = 4;
                    s->xC0 = func_00222B68(0.5235987901687622f,1.2217304706573486f);
                    CallTrackProjection(D_0029EA20,s->xC+16,s->xA0,s->xC+0xA0,1);
                } else s->x8=2;
            } else s->x8=1;
        } else {
            func_001733C0(s);
            func_00173688(s);
            distance=s->xB4;
            if (distance > 100.0f) func_00173560(s);
            else {
                v=s->x10->x874*distance*0.01f;
                __asm__ volatile("min.s %0, %2, %1" : "=&f"(minimum) : "f"(v), "f"(s->xB8));
                s->xB8=minimum;
            }
        }
    } else {
        func_00173358(s); func_001733C0(s); func_00173688(s); func_00173560(s);
    }
}
