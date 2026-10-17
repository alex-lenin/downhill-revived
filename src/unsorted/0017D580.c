/* UNIT 0x0017D580 0x0017D618 */
#include "object_registry.h"
typedef struct { int x0; unsigned short x4,x6; short x8[1]; } Obj;

void func_0017D580(Obj *obj,unsigned int *word) {
 unsigned short index;
 obj->x4=*word>>18;
 if((*word&0x3F)==11) {
 int value=RegisterObjectRetail((void *)(obj), (unsigned int)(word));
 *word=(*word&0xFFFC007F)|((value&0x7FF)<<7);
 } else { index=obj->x6; obj->x6=index+1; obj->x8[(short)index]=(*word>>7)&0x7FF; }
}
