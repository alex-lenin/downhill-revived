/* UNIT 0x0017D618 0x0017D6B0 */
#include "object_registry.h"
class Item { public: virtual void dispatch(int, int); };
struct Group { char pad[6]; short count; short items[1]; };


void func_0017D618(Group *self, int a, int b) {
    int i;
    for (i = 0; i < self->count; i++) {
        ((Item *)func_0017D3D0(self->items[i]))->dispatch(a, b);
    }
}
