/* UNIT 0x00225068 0x00225458 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x27C0];
    unsigned int x27C0;
    char pad27C4[0x27DA - 0x27C4];
    short x27DA;
    char pad27DC[0x27E0 - 0x27DC];
    float x27E0;
    char pad27E4[0x2814 - 0x27E4];
    int x2814;
    unsigned int x2818;
    int x281C;
    char pad2820[4];
    int x2824;
    unsigned char x2828, x2829, x282A, x282B;
    char pad282C[4];
    int *x2830;
    int *x2834;
    int *x2838;
    int *x283C;
    char pad2840[0x2854 - 0x2840];
    int x2854;
} Obj;

typedef struct Texture { int x0, x4, x8; unsigned short xC, xE; } Texture;

extern unsigned char D_002694D0[][4];
extern Texture D_0067DF90[];


extern int func_002249A8(int, int);

void func_00225068(Obj *obj)
{
    Texture *texture;
    int *buf;
    int *p;
    float *out;
    unsigned char *color;
    float scale;
    unsigned int n;
    int flag;
    int w, h;

    p = (int *)(((unsigned int)obj + 0xC) & 0xFFFFFFF0);
    buf = p;
    obj->x2854 = 0x100;
    flag = obj->x2824 == 0x44;
    p = (int *)func_002246A0((unsigned int *)(buf), obj->x2824, 0);
    *p++ = (obj->x2818 << 16) | 0x6C000148;
    color = D_002694D0[obj->x281C];
    scale = (float)obj->x282B * 0.0078125f;
    obj->x283C = p;
    out = (float *)p;
    if (obj->x27C0 & 0x100) {
        float red = obj->x2828;
        float green = obj->x2829;
        float blue = obj->x282A;
        n = obj->x2818;
        if (n != 0) {
            do {
                out[0] = red;
                out[1] = green;
                out[2] = blue;
                out[3] = color[3] * scale;
                color += 4;
                out += 4;
            } while (--n);
        }
    } else {
        n = obj->x2818;
        if (n != 0) {
            do {
                out[0] = color[0];
                out[1] = color[1];
                out[2] = color[2];
                out[3] = color[3] * scale;
                color += 4;
                out += 4;
            } while (--n);
        }
    }
    p += obj->x2818 * 4;
    *p++ = 0x6D044168;
    texture = &D_0067DF90[obj->x2814];
    if (obj->x27C0 & 0x80) {
        int a;
        h = (short)texture->xE >> 1;
        w = (short)texture->xC >> 1;
        a = (w + 9) | 0x90000;
        p[0] = 0x90009;
        p[1] = (w - 9) | ((h - 9) << 16);
        p[2] = a;
        p[3] = (w + (w - 9)) | ((h - 9) << 16);
        p[4] = ((h + 9) << 16) | 9;
        p[5] = (w - 9) | ((h + (h - 9)) << 16);
        p[6] = (w + 9) | ((h + 9) << 16);
        p[7] = (w + (w - 9)) | ((h + (h - 9)) << 16);
    } else {
        int y = (short)texture->xE - 9;
        int x = (short)texture->xC - 9;
        int a;
        p[0] = x | (y << 16);
        a = x | (9 << 16);
        p[1] = 9 | (9 << 16);
        p[2] = a;
        p[3] = 9 | (y << 16);
        p[4] = 9 | (y << 16);
        p[5] = a;
        p[6] = 9 | (9 << 16);
        p[7] = x | (y << 16);
    }
    p += 8;
    *p++ = 0x6802016C;
    if (flag)
        (int *)func_00224660((GsPacket *)(p), 0, 0x43431, 5, 0, 0x176, 1, 0, 0);
    else
        (int *)func_00224660((GsPacket *)(p), 0, 0x43431, 5, 0, 0x156, 1, 0, 0);
    ((float *)p)[4] = ((float)obj->x2818 - 1.0f) / ((float)obj->x27DA + 1.0f);
    ((float *)p)[5] = obj->x27E0;
    p[6] = 0;
    p[7] = 0;
    p += 9;
    obj->x2830 = p;
    p += 0x400;
    buf[1] = (int)p;
    p[2] = flag | 0x4000000;
    p[1] = 0;
    p += 4;
    obj->x2838 = p;
    p = (int *)((char *)p + (func_002249A8(obj->x2824, 0) * 4 + 0x200));
    obj->x2834 = p;
}
