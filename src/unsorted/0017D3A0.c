/* UNIT 0x0017D3A0 0x0017D3D0 */
#include "object_registry.h"
extern int D_0029DD8C;

int func_0017D3A0(void *value) {
    int index;
    void **entry;
    index = D_0029DD8C++;
    entry = &D_003B7758[index];
    *entry = value;
    return index;
}
