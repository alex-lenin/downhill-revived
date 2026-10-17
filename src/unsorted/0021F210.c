/* UNIT 0x0021F210 0x0021F3D8 */
typedef struct {
    char pad0[4];
    unsigned short x4;
    unsigned short x6;
    unsigned short x8;
    unsigned short xA;
    char padC[12];
    unsigned short x18;
    char pad1A[2];
    unsigned short x1C;
} Info;

typedef struct {
    char pad0[0x20];
    unsigned short x20;
    char pad22[2];
    unsigned short x24;
} Sub;

extern Info *func_0023DF20(int i);
extern int func_00238F30(void);

void func_0021F210(int arg0, char *arg1, int arg2, int arg3, int arg4)
{
    Info *info;
    Sub *dstA;
    Sub *dstB;
    int flag;

    info = func_0023DF20(arg0);
    flag = arg2 & 1;
    dstA = (Sub *)(arg1 + (flag ? 0x90 : 0x200));
    dstB = (Sub *)(arg1 + (flag ? 0x110 : 0x280));
    if (arg4 >= 3) {
        if (arg0 == 0) {
            dstA->x20 = dstB->x20 = (info->x18 - (info->x4 >> 1)) << 4;
            dstA->x24 = dstB->x24 = (info->x1C - (info->x6 >> 1)) << 4;
        } else if (arg0 == 1) {
            dstA->x20 = dstB->x20 = (0x800 - (info->x8 >> 1)) << 4;
            dstA->x24 = dstB->x24 = (info->x1C - (info->x6 >> 1)) << 4;
        } else if (arg0 == 2) {
            dstA->x20 = dstB->x20 = (info->x18 - (info->x4 >> 1)) << 4;
            dstA->x24 = dstB->x24 = (0x800 - (info->xA >> 1)) << 4;
        } else {
            dstA->x20 = dstB->x20 = (0x800 - (info->x8 >> 1)) << 4;
            dstA->x24 = dstB->x24 = (0x800 - (info->xA >> 1)) << 4;
        }
    } else {
        if (arg0 == 0 || arg0 == 5) {
            dstA->x20 = dstB->x20 = (info->x18 - (info->x4 >> 1)) << 4;
            dstA->x24 = dstB->x24 = (info->x1C - (info->x6 >> 1)) << 4;
        }
        dstA->x20 = dstB->x20 = (0x800 - (info->x8 >> 1)) << 4;
        dstA->x24 = dstB->x24 = (0x800 - (info->xA >> 1)) << 4;
    }
    if (arg3 != 0 && func_00238F30() == 0x3C) {
        dstA->x24 += 8;
        dstB->x24 += 8;
    }
}
