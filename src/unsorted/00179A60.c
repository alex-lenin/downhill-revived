/* UNIT 0x00179A60 0x00179E80 */
typedef struct Game {
    char pad0[0x40AC];
    signed char x40AC;
    char pad40AD[0x6390 - 0x40AD];
    int x6390;
    char pad6394[0x79A0 - 0x6394];
    int x79A0;
    char pad79A4[0x7ADC - 0x79A4];
    int x7ADC;
} Game;

typedef struct Obj {
    char pad0[0xF70];
    int xF70;
    char padF74[0xFC0 - 0xF74];
    void *xFC0;
    char padFC4[0x1164 - 0xFC4];
    Game *x1164;
    char pad1168[0x1170 - 0x1168];
    int x1170;
    char pad1174[0x126C - 0x1174];
    int x126C;
    int x1270;
    int x1274;
    char pad1278[0x1280 - 0x1278];
    int x1280;
    char pad1284[0x132C - 0x1284];
    int x132C;
    unsigned char x1330;
    char pad1331[0x135A - 0x1331];
    unsigned char x135A;
    unsigned char x135B;
    unsigned char x135C;
    unsigned char x135D;
    unsigned char x135E;
} Obj;

typedef struct Settings {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
} Settings;

extern Settings D_004B25A0;
extern char D_0029C9F0[]; /* "%s" */
extern char D_0029CA48[]; /* "Punch" */
extern char D_0029CA50[]; /* "Kick" */
extern char D_0029CA58[]; /* "Check" */
extern char D_0029CA60[]; /* "Stick" */
extern char D_0029CA68[]; /* "%s%d" */
extern char D_0029CA70[]; /* "Bottle" */
extern char D_0029CA78[]; /* "Mount" */
extern char D_0029CA80[]; /* "Repair" */

extern void func_0017A1B0(Obj *, int, unsigned char *, int, int, float);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_00218498(void *, int);
extern int func_0023DED0(void);
extern void func_0017B520(Obj *, char *, float, float);
extern int func_00254D68(char *, char *, ...);

void func_00179A60(Obj *obj)
{
    char buf[48];
    int kind;
    int show;
    float x, y, step;

    if (obj->x1170 != 0 && obj->xF70 != 0) {
        func_0017A1B0(obj, obj->x126C, &obj->x135A, obj->x1164->x40AC, 0, 0.3f);
        func_0017A1B0(obj, obj->x1274, &obj->x135C, obj->x1164->x79A0, 0, 0.3f);
        func_0017A1B0(obj, obj->x1270, &obj->x135B, 1, 0, 0.33f);
    }
    kind = obj->x1164->x6390;
    if (D_004B25A0.x288 > 1) {
        func_0020FB80(0, 0xF0, 0xB0, -0x7C, 0x10);
    } else {
        func_0020FB80(0, 0x100, 0xC0, -0x74, 0x10);
    }
    show = 1;
    if ((obj->x132C & 0x1F) > 16) {
        show = 0;
        if (obj->xFC0 != 0) {
            if (obj->x1330 != 0) {
                func_00218498(obj->xFC0, 2);
            } else {
                func_00218498(obj->xFC0, 1);
            }
        }
    } else if (obj->xFC0 != 0) {
        func_00218498(obj->xFC0, 0);
    }
    x = 1.84f;
    y = 1.14f;
    step = 0.12f;
    if (func_0023DED0() == 1) {
        x = 1.72f;
        y = 1.28f;
        step = 0.1f;
    } else if (func_0023DED0() == 2) {
        y = 0.64f;
        step = 0.205f;
    }
    if (show && obj->x135B >= 20 && D_004B25A0.x10C != 14) {
        func_0020FB80(0, 0xE0, 0xC0, -0x5B, -0x20);
        func_0020F588(obj->x1280, 0, 0x6E, 0x6E, 0xC8, 0xA0);
        if (kind == 0) {
            func_0017B520(obj, D_0029CA48, x, y + step * 2.0f);
        } else if (kind == 1) {
            func_0017B520(obj, D_0029CA50, x, y + step * 2.0f);
        } else if (kind == 2) {
            func_0017B520(obj, D_0029CA58, x, y + step * 2.0f);
        } else if (kind == 3) {
            func_0017B520(obj, D_0029CA60, x, y + step * 2.0f);
        } else if (kind == 4) {
            if (obj->x1164->x7ADC > 0) {
                func_00254D68(buf, D_0029CA68, D_0029CA70, obj->x1164->x7ADC);
            } else {
                func_00254D68(buf, D_0029C9F0, D_0029CA70, obj->x1164->x7ADC);
            }
            func_0017B520(obj, buf, x, y + step * 2.0f);
        }
    }
    if (obj->x132C != 0) {
        obj->x132C--;
    }
    if (obj->x135C >= 19) {
        if (obj->x135D < 150) {
            obj->x135D += 2;
        }
        func_0020F588(obj->x1280, 0, 0x78, 0x50, 0, obj->x135D);
        func_0017B520(obj, D_0029CA78, x, y + step);
    } else {
        obj->x135D = 0;
    }
    if (obj->x135A >= 19) {
        if (obj->x135E < 150) {
            obj->x135E += 2;
        }
        func_0020F588(obj->x1280, 0, 0x78, 0x14, 0, obj->x135E);
        func_0017B520(obj, D_0029CA80, x, y);
    } else {
        obj->x135E = 0;
    }
}
