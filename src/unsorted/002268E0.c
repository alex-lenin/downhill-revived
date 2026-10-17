/* UNIT 0x002268E0 0x002269E8 */
#include "gs_packet.h"
#include "particle.h"
typedef Particle Obj;

extern char D_E10[];


void func_002268E0(Obj *obj)
{
    unsigned int *packet;
    unsigned int *buf;
    int pad;
    int i;

    buf = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    packet = obj->x2830;
    packet[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    ((int *)packet)[-14] = obj->x2850;
    packet += obj->x2850 * 4;
    *buf = (((packet - buf) - 4) >> 2) | 0x20000000;
    buf = (unsigned int *)buf[1];
    buf[3] = ((unsigned char)obj->x2850 << 16) | 0x6E004108;
    packet = buf + 4 + obj->x2850;
    pad = obj->x2850 & 3;
    if (pad != 0) {
        for (i = 0; i < 4 - pad; i++) {
            *packet++ = 0;
        }
    }
    func_002249E0((GsPacket *)(packet), obj->x2824,(unsigned int)(D_E10),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
