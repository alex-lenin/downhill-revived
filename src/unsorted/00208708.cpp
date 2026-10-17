/* UNIT 0x00208708 0x00208788 */
#include "tween.h"
extern "C" int func_00202DA8(void);

extern "C" int func_00208708(FloatTween *obj) {
    float value;
    int active;

    value = ((float)(func_00202DA8() - obj->startTick) / (float)obj->duration) <? 1.0f;
    *obj->target = value * obj->delta + obj->start;
    active = 1;
    if (!(value < 1.0f)) active = 0;
    return active;
}
