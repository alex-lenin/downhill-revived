/* UNIT 0x002246A0 0x002249A8 */
#include "gs_packet.h"
typedef unsigned int u32;

u32 *func_002246A0(u32 *p, u32 code, int count) {
    u32 lo=code & 0xFFFF, hi=code>>16;
    p[1]=0;
    p[2]=0;
    switch(lo) {
    case 0x100:
        p[3]=(count+4)|0x50000000;
        p=(u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count+3);
        p[0]=0x5040B; p[1]=0; p[2]=0x47; p[3]=0;
        p[4]=0x1000000; p[5]=0; p[6]=0x4E; p[7]=0;
        p[8]=lo; p[9]=hi; p[10]=0x42; p[11]=0;
        return p+12;
    case 0x144:
    case 0x148:
        p[3]=(count+3)|0x50000000;
        p=(u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count+2);
        p[0]=0x5008B; p[1]=0; p[2]=0x47; p[3]=0;
        p[4]=lo; p[5]=hi; p[6]=0x42; p[7]=0;
        return p+8;
    case 1:
        p[3]=(count+3)|0x50000000;
        p=(u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count+2);
        p[0]=0x1000000; p[1]=lo; p[2]=0x4E; p[3]=0;
        p[4]=lo; p[5]=hi; p[6]=0x42; p[7]=0;
        return p+8;
    case 0:
        p[3]=(count+2)|0x50000000;
        p=(u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count+1);
        p[0]=0x1000000; p[1]=0; p[2]=0x4E; p[3]=0;
        return p+4;
    case 0x200:
        if(count) {
            p[3]=(count+1)|0x50000000;
            return (u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count);
        }
        return p+4;
    default:
        p[3]=(count+2)|0x50000000;
        p=(u32 *)func_00224660((GsPacket *)(p+4),0,14,1,0,0,0,1,count+1);
        p[0]=lo; p[1]=hi; p[2]=0x42; p[3]=0;
        return p+4;
    }
}
