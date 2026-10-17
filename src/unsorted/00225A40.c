/* UNIT 0x00225A40 0x00225B20 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x2600];
    float x2600;
    float x2604;
    float x2608;
    char pad260C[0x275C - 0x260C];
    unsigned int x275C;
    char pad2760[0x27C0 - 0x2760];
    unsigned int x27C0;
    char pad27C4[0x2824 - 0x27C4];
    int x2824;
    char pad2828[0x2830 - 0x2828];
    unsigned int *x2830;
    char pad2834[0x2850 - 0x2834];
    int x2850;
} Obj;

extern char D_1240[];


void func_00225A40(Obj *obj)
{
    unsigned int *packet;
    unsigned int buf;

    buf = ((unsigned int)obj + 0xC) & ~0xF;
    packet = obj->x2830;
    ((int *)packet)[-14] = obj->x2850;
    ((float *)packet)[-10] = obj->x2600;
    ((float *)packet)[-9] = obj->x2604;
    ((float *)packet)[-8] = obj->x2608;
    obj->x275C += 4;
    ((float *)packet)[-5] = obj->x275C;
    packet[-1] = ((unsigned char)obj->x2850 << 16) | 0x6C000008;
    func_002249E0((GsPacket *)(packet + obj->x2850 * 4), obj->x2824,(unsigned int)(D_1240),(GsPacket *)(buf), 0);
    obj->x27C0 |= 0x800;
}
