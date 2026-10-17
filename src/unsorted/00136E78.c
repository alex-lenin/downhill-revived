/* UNIT 0x00136E78 0x00136F80 */
#include "impulse.h"

typedef struct Sub158 {
    char pad0[0x7AC0];
    float x7AC0;
} Sub158;

typedef struct Obj {
    char pad0[0x150];
    float x150;
    char pad154[0x158 - 0x154];
    Sub158 *x158;
    char pad15C[0x418 - 0x15C];
    float x418;
    char pad41C[0x478 - 0x41C];
    float x478;
    float x47C;
    float x480;
} Obj;

extern int func_00138560(Obj *);

void func_00136E78(Obj *obj) {
    Vector4 force;
    float speed;
    float amount;
    float total;
    float limited;

    if (obj->x47C > 0.01f) {
        if (func_00138560(obj)) {
            obj->x478 = obj->x47C * 0.67f;
        } else {
            speed = obj->x158->x7AC0;
            if (speed < 4.0f) {
                obj->x478 = 0.0f;
            } else {
                total = obj->x47C;
                amount = total * (speed - 4.0f) * 0.2f;
                __asm__ volatile ("min.s %0, %1, %2" : "=&f"(limited) : "f"(total), "f"(amount));
                obj->x478 = limited;
            }
        }
        obj->x47C -= obj->x478;
        force.x = 0.0f;
        force.y = 1.75f;
        force.z = obj->x150 * obj->x418 * obj->x478 * obj->x480 * -20.0f;
        func_00231D48((ImpulseBody *)obj, &force);
    }
}
