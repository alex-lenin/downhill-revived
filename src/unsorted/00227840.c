/* UNIT 0x00227840 0x00227A60 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x27C0];
    unsigned int x27C0;
    char pad27C4[0x2824 - 0x27C4];
    int x2824;
    char pad2828[0x2830 - 0x2828];
    unsigned int *x2830;
    char pad2834[0x285C - 0x2834];
    signed char x285C;
    signed char x285D;
    signed char x285E;
} Obj;

extern char D_30F8[];


void func_00227840(Obj *obj)
{
    unsigned int *buf;
    unsigned int *p;
    unsigned int *r;
    int n0, n1, n2;
    int t, sh, sum;

    buf = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    n0 = obj->x285C;
    n1 = obj->x285D;
    n2 = obj->x285E;
    *buf = ((((obj->x2830 - buf) - 4) >> 2) + n0 * 2) | 0x20000000;
    t = n0 * 2 + 8;
    sum = obj->x285C + obj->x285D;
    sh = (n0 << 17) | 8;
    p = obj->x2830 - 23;
    sum += obj->x285E;
    p[-1] = sum;
    p[22] = sh | 0x6C000000;
    p = (unsigned int *)buf[1];
    p[0] = (n1 * 2) | 0x20000000;
    p[3] = t | (n1 << 17) | 0x6C000000;
    t += n1 * 2;
    p = (unsigned int *)p[1];
    p[0] = (n2 * 2) | 0x20000000;
    p[3] = t | (n2 << 17) | 0x6C000000;
    p = (unsigned int *)p[1];
    p[0] = ((n0 + 3) >> 2) | 0x20000000;
    p[3] = (n0 << 16) | 0x6E004108;
    r = p + 4 + n0;
    while ((unsigned int)r & 0xF)
        *r++ = 0;
    p = (unsigned int *)p[1];
    t = n0 + 0x108;
    p[0] = ((n1 + 3) >> 2) | 0x20000000;
    p[3] = t | (n1 << 16) | 0x6E004000;
    r = p + 4 + n1;
    while ((unsigned int)r & 0xF)
        *r++ = 0;
    t += n1;
    buf = (unsigned int *)p[1];
    buf[3] = t | (n2 << 16) | 0x6E004000;
    r = buf + 4 + n2;
    while ((unsigned int)r & 0xF)
        *r++ = 0;
    func_002249E0((GsPacket *)(r), obj->x2824,(unsigned int)(D_30F8),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
