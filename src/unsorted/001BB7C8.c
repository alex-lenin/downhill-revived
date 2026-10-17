/* UNIT 0x001BB7C8 0x001BB8D0 */
typedef struct Rider {
    char p0[0x1198];
    int x1198;
    int x119C;
    char p11A0[0xc0];
    int x1260;
    int x1264;
    char p1268[0x171];
    unsigned char x13D9;
    unsigned char x13DA;
    char p13DB[0x2ce5];
    char x40C0[0x160];
    int x4220;
    char p4224[0x1f0];
    unsigned char x4414;
    char p4415[0x7f];
    unsigned char x4494;
    char p4495[0x35e3];
    float x7A78;
} Rider;

typedef struct Watch {
    Rider *rider;
    char p4[8];
    int xC;
} Watch;

extern int func_001E51A8(void);
extern float func_00138580(char *);
extern float func_0012DEB0(Rider *);

int func_001BB7C8(Watch *w) {
    Rider *r;

    if (func_001E51A8()) {
        return 0;
    }
    r = w->rider;
    if (func_00138580(r->x40C0) < 3.0f && func_0012DEB0(r) == 0.0f && r->x7A78 == 0.0f
        && (r->x13D9 ? r->x1260 : r->x1198) == 0
        && (r->x13DA ? r->x1264 : r->x119C) == 0
        && r->x4414 != 2 && r->x4494 != 2 && r->x4220 == 0) {
        w->xC++;
        return w->xC >= 14;
    }
    w->xC = 0;
    return 0;
}
