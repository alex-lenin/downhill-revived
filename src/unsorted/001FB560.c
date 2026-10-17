/* UNIT 0x001FB560 0x001FB5A8 */
#include "object_registry.h"

void func_001FB560(void *obj, unsigned int *flags) {
    int value = func_0017D3A0((void *)(obj));
    *flags = (*flags & 0xFFFC007F) | ((value & 0x7FF) << 7);
}

