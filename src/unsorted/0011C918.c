/* UNIT 0x0011C918 0x0011C9C0 */
#include "motion_sample.h"
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Body {
    char pad0[0x793C];
    int x793C;
} Body;

typedef struct Racer {
    char pad0[0x2B4];
    Body *x2B4;
    char pad2B8[0x380 - 0x2B8];
} Racer;

typedef struct Result {
    float a;
    float b;
} Result;

extern Racer D_0037EA60[];

extern int func_0023D608(void);
extern Vec4 *func_0023CF78(int);
extern char *func_0023D180(int);

extern void func_0021D758(int, float);
void func_0011C918(void *arg)
{
    int i;
    Racer *racer;
    Body *body;
    Vec4 pos;
    char *info;
    float a;
    float b;

    racer = D_0037EA60;
    for (i = 0; i < func_0023D608(); i++) {
        body = racer->x2B4;
        pos = *func_0023CF78(i);
        racer++;
        info = func_0023D180(i);
        SampleMotionDiscard(arg, i, &a, info + 0xB8, &b, pos.z);
        func_0021D758(body->x793C, b);
    }
}
