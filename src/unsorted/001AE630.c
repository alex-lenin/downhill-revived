/* UNIT 0x001AE630 0x001AE660 */
#include "tween.h"
void func_001AE630(RiderBodyBlend *obj, int arg1, float value) {
    func_002086C8(&obj->tween, &obj->weight, arg1, obj->weight, value);
}
