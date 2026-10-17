/* UNIT 0x00206E40 0x00206E48 */
#include "animation.h"
struct S_00206E40 {
    char pad[0x4];
    float x4;
};

float func_00206E40(unsigned short *data, int unused, float time) {
    return ((struct S_00206E40 *)data)->x4;
}
