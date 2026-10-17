/* UNIT 0x002249E0 0x00224B88 */
#include "gs_packet.h"
typedef GsPacket Packet;

void func_002249E0(Packet *packet, unsigned int flags, unsigned int microAddress, Packet *start, int count) {
 packet->x4=(microAddress>>3)|0x14000000;
 packet->x8=0x11000000;
 packet->x0=0;
 if(flags&0x100) {
  packet->xC=(count+3)|0x50000000;
  packet=(Packet *)func_00224660((GsPacket *)(packet+1),0,14,1,0,0,0,1,count+2);
  packet->x0=0x5001B;
  packet->x8=0x47;
  packet->x4=0;
  packet->xC=0;
  packet++;
  packet->x0=0x01000000;
  packet->x4=1;
  packet->x8=0x4E;
  packet->xC=0;
  if(count) {
   packet++;
   packet->x8=8;
   *(GsSigned64 *)packet=0;
   packet->xC=0;
  }
 } else {
  if(flags<2) {
   packet->xC=0x50000002;
   packet=(Packet *)func_00224660((GsPacket *)(packet+1),0,14,1,0,0,0,1,1);
   packet->x4=1;
   packet->x0=0x01000000;
   packet->x8=0x4E;
  } else if(count) {
   packet->xC=0x50000002;
   packet=(Packet *)func_00224660((GsPacket *)(packet+1),0,14,1,0,0,0,1,1);
   *(GsSigned64 *)packet=0;
   packet->x8=8;
  }
  packet->xC=0;
 }
 start->x0=(packet-start)|0x60000000;
}
