/* UNIT 0x00226658 0x002268E0 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x2728];
    float x2728;
    char pad272C[0x27C8 - 0x272C];
    float x27C8;
    float x27CC;
    char pad27D0[4];
    float x27D4;
    char pad27D8[2];
    short x27DA;
    char pad27DC[0x27E0 - 0x27DC];
    float x27E0;
    float x27E4;
    float x27E8;
    float x27EC;
    char pad27F0[0x2814 - 0x27F0];
    int x2814;
    char pad2818[0x2824 - 0x2818];
    int x2824;
    char pad2828[0x2830 - 0x2828];
    int *x2830;
    int *x2834;
    int *x2838;
    char pad283C[0x2850 - 0x283C];
    int x2850;
    int x2854;
} Obj;

typedef struct Texture { int x0, x4, x8; unsigned short xC, xE; } Texture;

extern Texture D_0067DF90[];


extern int func_00219EE8(int, void *, void *);
extern int func_002249A8(int, int);

void func_00226658(Obj *obj)
{
    Texture *tex;
    int *buf;
    int *p;
    int *p2;
    int w;
    int h;
    int x0;
    int y0;
    int h1;

    p = (int *)(((unsigned int)obj + 12) & 0xFFFFFFF0);
    buf = p;
    obj->x2854 = 0x100;
    obj->x2850 = 0;
    p = (int *)func_002246A0((unsigned int *)(buf), obj->x2824, 1);
    tex = &D_0067DF90[obj->x2814];
    p2 = p + 4;
    func_00219EE8(tex->x8, tex, p2);
    p[0] = tex->x0;
    p[1] = tex->x4;
    p[2] = 6;
    p[3] = 0;
    p = p2;
    *p++ = 0x6D044208;
    w = (short)tex->xC;
    h = w >> 1;
    x0 = w - 8;
    y0 = h - 8;
    h1 = h + 8;
    p2[1] = 0x80008;
    p[1] = y0 | (y0 << 16);
    p[2] = h1 | 0x80000;
    p[3] = x0 | (y0 << 16);
    p[4] = 8 | (h1 << 16);
    p[5] = y0 | (x0 << 16);
    p[6] = h1 | (h1 << 16);
    p[7] = x0 | (x0 << 16);
    p2[9] = 0x6C03020C;
    p = (int *)func_00224660((GsPacket *)((int *)p2 + 10), 0, 0x43431, 5, 0, 0x176, 1, 0, 0);
    {
        float v;
        float t;
        float tmpD4;
        tmpD4 = obj->x27D4;
        v = (float)obj->x27DA;
        t = v * (1.0f - obj->x2728);
        ((float *)p)[-1] = tmpD4;
        ((float *)p)[0] = 1.0f / obj->x27E0;
        ((float *)p)[1] = obj->x27C8;
        ((float *)p)[2] = t / v;
        ((float *)p)[3] = obj->x27E8;
        ((float *)p)[4] = -1.0f / obj->x27E4;
        ((float *)p)[5] = (obj->x27CC - obj->x27C8) * 0.003921569f;
        ((float *)p)[6] = (v - t) / (v * 255.0f);
        ((float *)p)[7] = (obj->x27EC - obj->x27E8) * 0.003921569f;
        p[8] = 0;
    }
    p += 10;
    obj->x2830 = p;
    p = (int *)((char *)p + (obj->x2854 * 16));
    buf[1] = (int)p;
    p[1] = 0;
    p[2] = 0;
    p += 4;
    obj->x2838 = p;
    p += obj->x2854 + func_002249A8(obj->x2824, 0);
    obj->x2834 = p;
}
