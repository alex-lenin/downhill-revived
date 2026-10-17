/* UNIT 0x001AE318 0x001AE358 */
#include "tween.h"
struct Vec { float x0, x4, x8; };
struct Obj { char pad[0xA0]; struct Vec xA0; };

void func_001AE318(struct Obj *obj) {
    struct Vec *vec;
    func_001AE630((RiderBodyBlend *)(obj), 1, 1.0f);
    vec = &obj->xA0;
    vec->x0 = 0.0f;
    vec->x4 = 0.0f;
    vec->x8 = 0.0f;
}
