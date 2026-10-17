/* UNIT 0x001E41A0 0x001E4530 .rodata=0x294780:0x2947C0 */
struct Stats { char pad[0x50]; int x50; };
struct Obj {
    char pad[0x500];
    unsigned int x500;
    char pad504[0xC];
    int x510;
    char pad514[4];
    unsigned char x518;
    char pad519;
    unsigned char x51A;
    unsigned char x51B;
    unsigned char x51C;
    unsigned char x51D;
    unsigned char x51E;
    unsigned char x51F;
    int *x520;
    int *x524;
    int *x528;
    int x52C;
    int x530;
};
struct Rider {
    char pad[0x6610];
    char x6610[0x28];
    float x6638;
    char pad663C[0x7A69 - 0x663C];
    unsigned char x7A69;
};
struct G3 { char pad[0x50130]; struct Rider *rider; };

extern struct G3 D_002AF2C0;
extern char D_004B1F40[];
extern char D_00513B00[];
extern char *D_002718B8[];

extern void func_001B45F8(void *, int, int);
extern void func_001E5300(void *, int **, int **, int **, int **);
extern float func_0012E028(struct Rider *);
extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern int func_0020F860(int, char *);
extern void func_001E4950(struct Obj *, struct Obj *, int, int, int, int);
extern void func_002103D8(void *, int, struct Obj *);
extern void func_0020F9F0(int, int, int, int, char *);

int func_001E41A0(struct Obj *p) {
    int *a, *b, *c, *d;
    int width, height;
    int half, w, h;
    struct Rider *rider;

    if (p->x530 && p->x52C < 80) {
        p->x52C++;
    }
    if (p->x510 > 0) {
        if (--p->x510 == 0) {
            func_001B45F8(D_004B1F40, 1, 0);
            p->x510 = -1;
            return 1;
        }
    }
    rider = D_002AF2C0.rider;
    func_001E5300(rider->x6610, &a, &b, &c, &d);
    if (func_0012E028(rider) != 0.0f) {
        p->x51A = 1;
    }
    switch (p->x500) {
    case 0:
        if (rider->x7A69) p->x518 = 1;
        break;
    case 2:
        if (a[0x4C / 4] > 0) p->x518 = 1;
        break;
    case 3:
        if (a[0x40 / 4] > 0) p->x518 = 1;
        break;
    case 1:
        if (a[0x50 / 4] != 0 && p->x51A) p->x518 = 1;
        break;
    case 4:
        if (b[0x10 / 4] > 0) p->x518 = 1;
        break;
    case 5:
        if (b[0x44 / 4] > 0) p->x518 = 1;
        break;
    case 8:
        if (a[0x28 / 4] > 0 && rider->x6638 > 6.0f) p->x518 = 1;
        break;
    case 9:
        if (c[2] > 0 && p->x51F) p->x518 = 1;
        break;
    case 10:
        if (c[2] >= 2 && p->x51B && p->x51F) p->x518 = 1;
        break;
    case 11:
        if (p->x51C) p->x518 = 1;
        break;
    case 12:
        if (p->x51E) p->x518 = 1;
        break;
    case 13:
        if (p->x51D) p->x518 = 1;
        break;
    case 14:
        if (p->x51C && p->x51A) p->x518 = 1;
        break;
    }
    func_0023D650(0, &width, &height);
    half = width / 2;
    h = 16;
    func_0020FB80(0, 224, 192, -75, -16);
    func_0020F588(0, 0, 110, 110, 200, p->x52C);
    w = func_0020F860(0, D_002718B8[p->x500]) >> 4;
    if (w <= 0) w = 0;
    if (p->x500 == 14) {
        h = 38;
    } else if (p->x500 == 9 || p->x500 == 10) {
        h = 49;
    } else if (p->x500 == 7 || p->x500 == 0) {
        h = 27;
    }
    func_001E4950(p, p, half - (w >> 1) - 6, 48, w + 14, h);
    *p->x520 = 127;
    *p->x524 = 127;
    *p->x528 = 40;
    func_002103D8(D_00513B00, 0, p);
    func_0020F9F0(0, 0, half, 50, D_002718B8[p->x500]);
    return 1;
}
