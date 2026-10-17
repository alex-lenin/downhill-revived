/* UNIT 0x001FB508 0x001FB550 */
#include "object_registry.h"

void func_001FB508(void *obj, unsigned int *flags) {
    int value = func_0017D3A0((void *)(obj));
    *flags = (*flags & 0xFFFC007F) | ((value & 0x7FF) << 7);
}
