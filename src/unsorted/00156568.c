/* UNIT 0x00156568 0x00156580 */
#include "rider_geometry.h"
typedef struct RiderSpeedScale {
    char pad0[0xA0];
    float xA0;
} Obj;

void func_00156568(Obj *self, float value)
{
    self->xA0 = 1.0f / value;
}
