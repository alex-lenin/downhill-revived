/* UNIT 0x00226B90 0x00226C10 */
#include "gs_packet.h"
#include "particle.h"
typedef Particle Obj;

extern char D_17C8[];


void func_00226B90(Obj *obj)
{
    unsigned int *packet;

    packet = obj->x2830;
    obj->x2854 = 0x81;
    packet[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    func_002249E0((GsPacket *)(packet + obj->x2850 * 4), obj->x2824,(unsigned int)(D_17C8),(GsPacket *)(((unsigned int)obj + 0xC) & ~0xF), 0);
    obj->x27C0 |= 0x800;
}
