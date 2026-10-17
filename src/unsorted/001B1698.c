/* UNIT 0x001B1698 0x001B16C8 */
#include "tween.h"
int func_001B1698(RiderBlendController *obj, int frames, float weight) {
    return func_002086C8(&obj->tween, &obj->weight, frames, obj->weight, weight);
}
