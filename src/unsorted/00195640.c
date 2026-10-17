/* UNIT 0x00195640 0x001956C0 */
#include "rtc.h"
struct Local { unsigned char bytes[16]; };
struct Obj { char pad[0xbc]; int xbc; short xc0, xc2; unsigned char xc4, xc5, xc6, xc7; };

void func_00195640(struct Obj *obj, int value, int a, int b, int c) {
 struct Local local;
 ReadRtcDiscard(&local);
 obj->xbc = value; obj->xc0 = a; obj->xc2 = b;
 obj->xc4 = local.bytes[5]; obj->xc5 = local.bytes[6]; obj->xc6 = local.bytes[7]; obj->xc7 = c;
}
