/* UNIT 0x00224F58 0x00225068 */
#include "gs_packet.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x2770];
    unsigned int x2770;
    char pad2774[0x2780 - 0x2774];
    float x2780[4];
    char pad2790[0x27C0 - 0x2790];
    unsigned int x27C0;
    char pad27C4[0x2824 - 0x27C4];
    int x2824;
    char pad2828[0x2830 - 0x2828];
    unsigned int *x2830;
    unsigned int *x2834;
    char pad2838[0x2850 - 0x2838];
    int x2850;
} Obj;

extern float D_0027A4A0[];
extern char D_50[];


void func_00224F58(Obj *obj)
{
    unsigned int *packet;
    unsigned int *buf;

    buf = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    *buf = ((((obj->x2830 - buf) - 4) >> 2) + obj->x2850) | 0x20000000;
    packet = obj->x2830;
    ((int *)packet)[-12] = obj->x2850;
    if (obj->x2770 & 0x400) {
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                          "lqc2 $vf12, 0x0(%2)\n\t"
                          "mfc1 $2, %3\n\t"
                          "qmtc2.ni $2, $vf13\n\t"
                          "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                          "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                          "sqc2 $vf11, %0"
                          : "=m"(*(u128 *)(packet - 8))
                          : "r"(obj->x2780), "r"(D_0027A4A0), "f"(obj->x2780[3]) : "$2");
    } else {
        /* sq $zero: gcc always materializes a TImode 0 with por first */
        __asm__ volatile ("sq $0, %0" : "=m"(*(u128 *)(packet - 8)));
    }
    packet[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    packet = obj->x2834;
    buf = packet - 4;
    buf[3] = ((unsigned char)obj->x2850 << 16) | 0x6C000108;
    func_002249E0((GsPacket *)(packet + obj->x2850 * 4), obj->x2824,(unsigned int)(D_50),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
