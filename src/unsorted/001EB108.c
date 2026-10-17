/* UNIT 0x001EB108 0x001EB158 */
typedef struct Obj { char pad0[0x11AC]; int x11AC; char pad1[0xC4]; int x1274; char pad2[0x166]; unsigned char x13DE; char pad3[0x1AB5]; float x2E94[8]; } Obj;
int func_001EB108(Obj **arg) {
    Obj *p = *arg;
    int value;
    float *f;
    int less;
    float first, second;
    if (p->x13DE != 0) value = p->x1274; else value = p->x11AC;
    if (value != 0) {
        f = p->x2E94;
        second = f[1]; first = f[7];
        less = 1;
        if (!(second > first)) less = 0;
        if (less & 255) return 1;
    }
    return 0;
}
