/* UNIT 0x00227518 0x00227618 */
#include "gs_packet.h"
#include "particle.h"
typedef Particle Obj;

extern char D_2E48[];


void func_00227518(Obj *obj)
{
    unsigned int *buf;
    unsigned int *p;
    unsigned int i;

    buf = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    p = obj->x2830;
    p[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    ((int *)p)[-19] = obj->x2850;
    p += obj->x2850 * 4;
    *buf = (((p - buf) - 4) >> 2) | 0x20000000;
    buf = (unsigned int *)buf[1];
    buf[3] = ((unsigned char)obj->x2850 << 16) | 0x6E000108;
    p = &buf[obj->x2850 + 4];
    for (i = (unsigned int)p; i & 0xC; i += 4) {
        *p++ = 0;
    }
    func_002249E0((GsPacket *)(p), obj->x2824,(unsigned int)(D_2E48),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
