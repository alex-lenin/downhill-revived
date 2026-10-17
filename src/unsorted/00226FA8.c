/* UNIT 0x00226FA8 0x00227310 */
#include "gs_packet.h"
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Color {
    unsigned char r, g, b, a;
} Color;

typedef struct Obj {
    char pad0[0x2754];
    float x2754;
    char pad2758[0x27C8 - 0x2758];
    float x27C8;
    float x27CC;
    char pad27D0[0x27DC - 0x27D0];
    float x27DC;
    float x27E0;
    float x27E4;
    float x27E8;
    float x27EC;
    char pad27F0[0x2810 - 0x27F0];
    unsigned int x2810;
    char pad2814[0x281C - 0x2814];
    int x281C;
    char pad2820[0x2824 - 0x2820];
    int x2824;
    char pad2828[0x2830 - 0x2828];
    u128 *x2830;
    u128 *x2834;
    char pad2838[0x283C - 0x2838];
    float *x283C;
    char pad2840[0x2850 - 0x2840];
    int x2850;
} Obj;

extern Color D_002694D0[];
extern char D_22C8[];
extern void func_00226D08(Obj *obj);



extern int func_002249A8(int count, int flags);

void func_00226FA8(Obj *obj)
{
    unsigned int *p;
    unsigned int *base;
    float *f;
    Color *color;
    float one;
    float range;
    float scale;
    float offset;
    float x27EC;
    float x27E4;
    int i;

    if (((obj->x2810 & 0xFFFFFF8F) ^ 0x8008) == 0) {
        func_00226D08(obj);
        return;
    }
    x27E4 = obj->x27E4;
    one = 1.0f;
    x27EC = obj->x27EC;
    scale = obj->x27C8;
    range = (one - x27EC) / (scale * (x27E4 - obj->x27E0));
    offset = one - scale * x27E4 * range;
    p = (unsigned int *)(((unsigned int)obj + 0xC) & ~0xF);
    base = p;
    color = &D_002694D0[obj->x281C];
    obj->x2850 = 10;
    p = (unsigned int *)func_002246A0((unsigned int *)(base), obj->x2824, 1);
    *(long *)p = 5;
    p[2] = 8;
    p[3] = 0;
    p += 4;
    p[3] = 0x6C0A0008;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p = (unsigned int *)func_00224660((GsPacket *)(p + 4), 0, 0x51252512, 8, 0, 0x53, 1, 0, 0);
    obj->x283C = f = (float *)p;
    f[0] = 0.0625f;
    f[1] = one / obj->x27C8;
    range *= 0.5f;
    f[2] = obj->x27C8 * 0.0625f;
    f[3] = obj->x27C8;
    f[4] = 1.11f;
    f[5] = 1.31f;
    f[6] = 1.543211f;
    f[7] = 1.711f;
    p[8] = color->r;
    p[9] = color->g;
    p[10] = color->b;
    f[12] = obj->x2754 * 8.0f;
    f[13] = obj->x2754 * -12.0f;
    p[14] = 0;
    f[15] = obj->x2754 * 4.0f;
    f[16] = obj->x27CC * 0.5f;
    f[17] = -obj->x27CC * 0.5f;
    f[18] = obj->x27CC * 0.28867513f;
    f[19] = obj->x27CC * 0.28867513f;
    f[20] = one;
    f[22] = obj->x27E8 * 0.5f;
    f[23] = range;
    f[24] = -0.3660254f;
    p[25] = 0;
    f[26] = one;
    f[27] = -obj->x27DC;
    f[28] = 1.3660254f;
    p[29] = 0;
    f[30] = one;
    f[31] = offset;
    f[32] = 0.5f;
    f[33] = 1.5f;
    f[34] = one;
    f[35] = x27EC;
    p += 36;
    func_002249E0((GsPacket *)(p), obj->x2824,(unsigned int)(D_22C8),(GsPacket *)(base), 1);
    p += func_002249A8(obj->x2824, 1);
    obj->x2830 = (u128 *)(p + 16);
    for (i = 4; i < 8; i++) {
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x2830[i]));
    }
    obj->x2834 = &obj->x2830[i];
    for (i = 4; i < 8; i++) {
        __asm__ volatile ("sqc2 $vf0, %0" : "=m"(obj->x2834[i]));
    }
}
