/* UNIT 0x00206E48 0x00206E90 */
#include "animation_query.h"
#include "animation.h"
extern AnimationHandler D_004D0278[][3];
#if defined(__mips__)
float func_00206E48(unsigned short *arg0, int arg1, float time)
#else
float SharedCall00206E48(unsigned short * arg0, int arg1, float time)
#endif
{

    unsigned short flags = *arg0;
    return D_004D0278[flags & 7][(flags >> 3) & 7](arg0, arg1, time);
}
