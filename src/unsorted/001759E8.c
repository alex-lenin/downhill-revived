/* UNIT 0x001759E8 0x00175A78 */
typedef struct { char pad0[4]; char x4; } Flag;
typedef struct { char pad0[0xc84]; int xC84[4]; char padC94[0x250]; int *xEE4[2]; char padEEC[0x280]; Flag *x116C; } Obj;
extern int D_004B2828[];
extern void func_00178170(Obj *);
void func_001759E8(Obj *obj) {
    int *src;
    int **dst;
    int count;
    if (D_004B2828[0] < 3) {
        src = obj->xC84;
        dst = obj->xEE4;
        count = 1;
        do {
            count--;
            (*dst)[0] = obj->xC84[0];
            (*dst)[1] = src[1];
            (*dst)[2] = src[2];
            (*dst)[3] = src[3];
            dst++;
        } while (count >= 0);
        func_00178170(obj);
        if (obj->x116C) obj->x116C->x4 = 0;
    }
}
