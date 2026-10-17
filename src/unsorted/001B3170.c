/* UNIT 0x001B3170 0x001B31A0 */
#include "tween.h"
void func_001B3170(RiderSecondaryBlend *obj, int value, float amount) {
    func_002086C8(&obj->tween, &obj->weight, value, obj->weight, amount);
}
