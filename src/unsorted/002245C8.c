/* UNIT 0x002245C8 0x00224660 .sdata=0x29DB30:0x29DB58 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Packet {
    unsigned long pad0 : 48;
    unsigned long x0_48 : 3;
    unsigned long pad0_51 : 13;
    unsigned short x8;
    char padA[0x46];
    u128 x50;
} Packet;

extern u128 D_006785C0[];
extern int D_0029E2EC;

unsigned short D_0029DB30 = 0;
unsigned int D_0029DB34 = 0;
Packet *D_0029DB38 = 0;
unsigned char D_0029DB3C = 0;
int D_0029DB40 = 0;
int D_0029DB44 = 0;
int D_0029DB48 = 0;
int D_0029DB4C = 0;
int D_0029DB50 = 0;
int D_0029DB54 = 0;

int func_002245C8(void) {
    Packet *packet;

    packet = D_0029DB38;
    if (D_0029DB3C == 0) {
        return 1;
    }
    if (packet != 0) {
        packet->x50 = D_006785C0[0];
        packet->x0_48 = D_0029DB34;
        packet->x8 = D_0029DB30;
        D_0029DB38 = 0;
        D_0029E2EC = 0;
        D_0029DB3C = 0;
        return 0;
    }
    return 1;
}
