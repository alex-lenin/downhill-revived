/* UNIT 0x00225DB0 0x00226588 */
#include "gs_packet.h"
typedef struct Obj {
    char pad0[0x2830];
    int *x2830;
    int *x2834;
    short *x2838;
    char pad283C[0x2854 - 0x283C];
    int x2854;
} Obj;

typedef struct Texture {
    int x0, x4, x8;
    short xC, xE;
} Texture;

typedef struct B4 { unsigned char v[4]; } B4;
typedef struct B54 { unsigned char a[18], b[18], c[18]; } B54;
typedef struct F18 { float v[18]; } F18;
typedef struct B18 { unsigned char v[18]; } B18;

extern B4 D_0029DB58[];
extern B54 D_00298EB0;
extern F18 D_00298EE8;
extern B18 D_00298F30;
extern F18 D_00298F48;
extern F18 D_00298F90;
extern Texture D_0067DF90[];
extern int *D_00723A10[];

extern void func_0023D618(int, int *, int *);
extern void func_0023D650(int, int *, int *);


void func_00225DB0(Obj *obj, int arg1)
{
    B4 unk10;
    int unk20[4];
    B54 unk30;
    F18 unk70;
    B18 unkC0;
    F18 unkE0;
    F18 unk130;
    int x, y, w, h;
    int *p;
    unsigned char *pc0;
    float *pe0;
    float *p130;
    int *p20;
    float *p70;
    int i, j, count;

    unk10 = D_0029DB58[0];
    unk30 = D_00298EB0;
    unk70 = D_00298EE8;
    unkC0 = D_00298F30;
    unkE0 = D_00298F48;
    unk130 = D_00298F90;

    for (i = 0; i < 4; i++)
        unk20[i] = D_0067DF90[unk10.v[i]].xC - 9;

    obj->x2854 = 74;
    p = (int *)(((unsigned int)obj + 12) & 0xFFFFFFF0);
    p[0] = 0;
    p[1] = 0;
    p[2] = 0x6C25000C;
    func_0023D618(arg1, &x, &y);
    p += 3;
    func_0023D650(arg1, &w, &h);
    p130 = unk130.v;
    pe0 = unkE0.v;
    pc0 = unkC0.v;
    ((float *)p)[0] = (float)x;
    ((float *)p)[1] = (float)y;
    p[2] = 18;
    p[3] = 0;
    p += 4;
    for (i = 17; i >= 0; i--) {
        p[0] = *pc0;
        p[1] = unk20[*pc0++];
        ((float *)p)[2] = *pe0++;
        ((float *)p)[3] = *p130++;
        p += 4;
    }
    {
        int *q = p;
        p70 = unk70.v;
        for (i = 0; i < 18; i++) {
            ((float *)q)[0] = (float)unk30.a[i];
            ((float *)q)[1] = (float)unk30.b[i];
            ((float *)q)[2] = (float)unk30.c[i];
            ((float *)q)[3] = *p70++;
            q += 4;
        }
        p = q;
    }
    p[0] = 0x6C09010F;
    p = (int *)func_00224660((GsPacket *)(p + 1), 0, 0x551E, 4, 0, 0x46, 1, 1, 1);
    p[0] = 0x44;
    p[1] = 0x20;
    p[2] = 0x42;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    obj->x2834 = p + 7;
    p[8] = (x - w) * 16;
    p[9] = (y - h) * 16;
    p[10] = 0xFFFFFF;
    p[11] = 0;
    p[12] = (x + w) * 16;
    p[13] = (y + h) * 16;
    p[14] = 0xFFFFFF;
    p[15] = 0;
    p = (int *)func_00224660((GsPacket *)(p + 16), 0, 0xE, 1, 0, 0, 0, 0, 2);
    p[0] = 0x48;
    p[1] = 0x20;
    p[2] = 0x42;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 6;
    p[7] = 0;
    p = (int *)func_00224660((GsPacket *)(p + 8), 0, 0x53531, 5, 0, 0x156, 1, 1, 0);
    p[0] = 0x6804028A;
    p = (int *)func_00224660((GsPacket *)(p + 1), 0, 0xE, 1, 0, 0, 0, 0, 2) - 1;
    p[0] = 0x48;
    p[1] = 0x20;
    p[2] = 0x42;
    p[3] = 0;
    p[4] = 0;
    p[5] = 6;
    p = (int *)func_00224660((GsPacket *)(p + 6), 0, 0x53531, 5, 0, 0x156, 1, 1, 0);
    p--;
    for (count = (3 - ((unsigned int)p >> 2)) & 3; count != 0; count--)
        *p++ = 0;
    p++;
    obj->x2830 = p;
    D_00723A10[arg1] = p;
    p = (int *)((char *)p + 0xDF0);
    obj->x2838 = (short *)p;
    for (i = 0; i < 4; i++)
        obj->x2838[i] = unk10.v[i];
}
