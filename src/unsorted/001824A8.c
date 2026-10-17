/* UNIT 0x001824A8 0x00182580 .rodata=0x28C540:0x28C590 */
typedef struct Obj {
    unsigned int flags;
} Obj;

extern Obj *D_0029E4BC;
extern int D_004B2840[];
extern int D_0029DD90[];
extern int D_0029DD98[];
extern int D_0029DDA0[];
extern int D_0029DDA8[];
extern int D_0029DDB0[];
extern int D_0029DDB8[];
extern int D_0029DDC0[];
extern int D_0029DDC8[];
extern int D_0029DDD0[];

int *func_001824A8(Obj *obj)
{
    int mode;

    D_0029E4BC = obj;
    obj->flags &= 0xFFFC007F;
    mode = D_004B2840[0];
    switch (mode) {
    case 0:
        D_0029DD90[0] = mode;
        return D_0029DD90;
    case 15:
        D_0029DD98[0] = mode;
        return D_0029DD98;
    case 19:
        D_0029DDA0[0] = mode;
        return D_0029DDA0;
    case 18:
        D_0029DDA8[0] = mode;
        return D_0029DDA8;
    case 12:
        D_0029DDB0[0] = mode;
        return D_0029DDB0;
    case 5:
        D_0029DDB8[0] = mode;
        return D_0029DDB8;
    case 2:
        D_0029DDC0[0] = mode;
        return D_0029DDC0;
    case 3:
        D_0029DDC8[0] = mode;
        return D_0029DDC8;
    default:
        D_0029DDD0[0] = mode;
        return D_0029DDD0;
    }
}
