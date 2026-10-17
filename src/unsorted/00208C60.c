/* UNIT 0x00208C60 0x00208D68 */
#include "sound_math.h"
extern float func_00248D90(float, float);

/* Heading (degrees, 0..359) from b to a, relative to heading (radians); 0 if a == b in x/y. */
int func_00208C60(float *b, float *a, float heading)
{
    float v[4] __attribute__((aligned(16)));
    float angle;

    heading = heading * 57.29578f;
    if (heading < 0.0f) {
        heading += 360.0f;
    }
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(v), "r"(a), "r"(b) : "memory");
    angle = func_00248D90(v[0], v[1]) * 57.29578f;
    if (v[0] == 0.0f && v[1] == 0.0f) {
        return 0;
    }
    angle -= heading;
    if (angle < 0.0f) {
        angle += 360.0f;
    } else if (angle > 359.0f) {
        angle -= 360.0f;
    }
    return (int)angle;
}
