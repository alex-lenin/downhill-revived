/* UNIT 0x0016C720 0x0016C7C8 */
#include "motion_sample.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

struct Rider { char pad0[0x793C]; int x793C; };

struct Player {
    char pad0[0x2B4];
    struct Rider *rider;
    char pad2B8[0x380 - 0x2B8];
};

extern struct Player D_0037EA60[];
extern int func_0023D608(void);
extern Vec4 *func_0023CF78(int);
extern char *func_0023D180(int);

extern void func_0021D758(int, float);

void func_0016C720(void *arg0)
{
    struct Player *player = D_0037EA60;
    int i;

    for (i = 0; i < func_0023D608(); i++) {
        struct Rider *rider = player->rider;
        Vec4 pos;
        char *info;
        float out;
        float value;

        player++;
        pos = *func_0023CF78(i);
        info = func_0023D180(i);
        SampleMotionDiscard(arg0, i, &out, info + 0xB8, &value, pos.z);
        func_0021D758(rider->x793C, value);
    }
}
