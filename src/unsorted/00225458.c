/* UNIT 0x00225458 0x00225550 */
#include "gs_packet.h"
#include "particle.h"
typedef Particle Obj;

extern char D_2D0[];


void func_00225458(Obj *obj)
{
    unsigned int *packet;
    unsigned int *buf;
    int qwc;

    buf = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    *buf = ((((obj->x2830 - buf) - 4) >> 2) + obj->x2850) | 0x20000000;
    packet = obj->x2830;
    buf = packet + 0x400;
    ((int *)packet)[-9] = obj->x2850;
    ((int *)packet)[-6] = obj->x2850 >> 2;
    packet[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    qwc = (obj->x2850 + 3) >> 2;
    buf[3] = (qwc << 16) | 0x6D000108;
    packet = buf + (qwc * 2 + 4);
    if (qwc & 1) {
        packet[0] = 0;
        packet[1] = 0;
        packet += 2;
    }
    func_002249E0((GsPacket *)(packet), obj->x2824,(unsigned int)(D_2D0),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
