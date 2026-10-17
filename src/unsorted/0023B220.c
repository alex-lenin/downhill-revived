/* UNIT 0x0023B220 0x0023B248 */
#include "scalar_api.h"
void func_0023B220(unsigned long *out, unsigned long a, unsigned long b, unsigned long c) {
    *out = (a << 32) | ((b << 32) >> 4) | ((c << 32) >> 32);
}
