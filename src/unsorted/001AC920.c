/* UNIT 0x001AC920 0x001ACA08 */
#include "animation.h"
#include "timer.h"
typedef AnimationHandle Handle;

typedef AnimationController Anim;

extern char D_00759780[];
extern void func_002625A0(void);
extern void func_001AC810(Anim *anim, float value);


void func_001AC920(Anim *anim, float value, float time)
{
    Handle handle;

    if (anim->x0 && anim->x0->x0) {
        func_001AC810(anim, value);
        if (time > 0.0f) {
            anim->x5C = -anim->x44 * 16.0f / time;
        } else {
            anim->x5C = -anim->x44;
        }
        handle = *anim->x0;
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_002625A0), anim, (int)func_00204280(&handle));
    }
}
