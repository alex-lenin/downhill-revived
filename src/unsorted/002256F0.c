/* UNIT 0x002256F0 0x002257F8 */
#include "gs_packet.h"
#include "particle.h"
typedef Particle Obj;

extern char D_1010[];


void func_002256F0(Obj *obj)
{
    unsigned int *tag;
    unsigned int *packet;

    tag = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    packet = obj->x2830;
    *tag = (obj->x2850 + (((int)packet - (int)tag) >> 4) - 1) | 0x20000000;
    packet[-9] = obj->x2850;
    packet[-1] = (obj->x2850 << 16) | 0x6C000008;
    packet += obj->x2854 * 4;
    packet[0] = obj->x2850 | 0x20000000;
    packet[3] = (obj->x2850 << 16) | 0x6C000088;
    packet += obj->x2854 * 4 + 4;
    tag = packet;
    tag[3] = (obj->x2850 << 16) | 0x6C000108;
    packet += obj->x2850 * 4 + 4;
    func_002249E0((GsPacket *)(packet), obj->x2824,(unsigned int)(D_1010),(GsPacket *)(tag), 0);
    obj->x27C0 |= 0x800;
}
