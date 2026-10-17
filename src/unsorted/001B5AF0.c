/* UNIT 0x001B5AF0 0x001B5B40 */
#include "scalar_api.h"
extern void func_001B5B40(void *);
void func_001B5AF0(unsigned char *obj, int offset, int value) {
    if (obj[0x61]) func_001B5B40(obj);
    obj[offset] = value;
}
