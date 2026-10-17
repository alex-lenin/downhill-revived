/* UNIT 0x00224660 0x002246A0 */
#include "gs_packet.h"
typedef GsPacket Packet;
Packet *func_00224660(Packet *packet, unsigned int regsHigh, unsigned int regsLow, unsigned int nreg, unsigned int format, unsigned int prim, unsigned int pre, unsigned int eop, unsigned int nloop) {
    packet->x0 = nloop | (eop << 15);
    packet->x4 = (pre << 14) | (prim << 15) | (format << 26) | (nreg << 28);
    packet->x8 = regsLow;
    packet->xC = regsHigh;
    return packet + 1;
}
