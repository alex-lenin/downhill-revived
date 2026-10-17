/* UNIT 0x0017CC58 0x0017CCB0 */
#include "object_registry.h"
typedef struct { unsigned int flags; char pad[24]; float x1C; } Entry;
typedef struct { char pad[24]; Entry *x18; int x1C; int x20; float x24; } Obj;

void func_0017CC58(Obj *obj) {
 int result;
 Entry *entry;
 obj->x24=obj->x18->x1C;
 result=RegisterObjectRetail((void *)(obj), (unsigned int)(obj->x1C));
 entry=obj->x18;
 entry->flags=(entry->flags & 0xFFFC007F) | ((result & 0x7FF)<<7);
}
