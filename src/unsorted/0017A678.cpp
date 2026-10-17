/* UNIT 0x0017A678 0x0017B0D8 */
#include "race_limit.h"
#include "format.h"
struct Rider {
    char pad0[0x661C];
    float x661C;
    char pad6620[0x798C - 0x6620];
    int x798C;
    char pad7990[0x7BD0 - 0x7990];
};

struct World {
    Rider riders[8];
    char pad[0x501A8 - 8 * 0x7BD0];
    int x501A8;
};

struct Material {
    char pad0[0x4C];
    float x4C;
};

struct Drawable {
    char pad0[0x20];
    Material *x20;
};

struct Materials {
    char pad0[8];
    unsigned short x8;
    char padA[0x20 - 0xA];
    Material *x20[1];
};

struct Obj {
    char pad0[0xF94];
    Materials *xF94;
    Drawable *xF98;
    char padF9C[0x1164 - 0xF9C];
    Rider *x1164;
    char pad1168[0x1184 - 0x1168];
    Drawable *x1184;
    char pad1188[0x1220 - 0x1188];
    float x1220, x1224, x1228;
    char pad122C[0x1280 - 0x122C];
    int x1280;
    char pad1284[0x135F - 0x1284];
    unsigned char x135F, x1360;
    char pad1361[3];
    int x1364;
};

struct Game {
    char pad0[0x10C];
    int x10C, x110;
    char pad114[0x288 - 0x114];
    int x288;
    char pad28C[4];
    int x290;
    char pad294[0x2A0 - 0x294];
    int x2A0;
    int getMode() { return x10C; }
};

struct Mode {
    int x0, x4, x8, xC, x10, x14;
    int get8() { return x8; }
    int getC() { return xC; }
};

struct ModeStats {
    char pad0[0x44];
    int x44;
    char pad48[8];
};

struct Profile {
    ModeStats modes[21];
    char pad690[4];
};

struct SaveData {
    Profile profiles[8];
    char pad[0x1219C - 8 * 0x694];
    unsigned char x1219C, x1219D, x1219E;
    Profile *getProfile(int index) { return &profiles[index]; }
};

struct Names {
    char pad0[0x1C];
    int ids[8];
    char pad3C[8];
    int x44;
    int *getNames() { return ids; }
};

extern World D_002AF2C0;
extern Game D_004B25A0;
extern Mode D_00381CF0[];
extern SaveData D_004225A0;
extern Names D_0043EEC0;
extern char *D_0026EFE8[];
extern char D_0028B928[], D_0028BD20[], D_0028BC90[];
extern char D_0028BD30[], D_0028BD48[], D_0028BD60[], D_0028BD78[];
extern char D_0028BD90[], D_0028BDA8[], D_0028BDC0[], D_0028BDD8[];
extern char D_0028BDE8[], D_0028BDF8[], D_0028BE18[], D_0028BE28[];
extern char D_0028BE40[], D_0028BE60[], D_0028BE80[], D_0028BEA0[];
extern char D_0028BEB8[], D_0028BEC8[], D_0028BEE0[], D_0028BF08[];
extern char D_0028BF18[], D_0028BD08[];

extern "C" void func_0017A328(Obj *, int);
extern "C" void func_00176930(Obj *, Drawable *, float *, float, float, float);
extern "C" int func_0014EA70(Mode *);
extern "C" int func_0014EC78(Mode *);

extern "C" int func_00194968(SaveData *, int);
extern "C" int func_00194C48(Profile *);
extern "C" int func_00194940(SaveData *, int);
extern "C" int func_00194430(SaveData *, int, int);
extern "C" void func_0023D650(int, int *, int *);
extern "C" int func_0023DED0();
extern "C" void func_0020FB80(int, int, int, int, int);
extern "C" void func_0020F588(int, int, int, int, int, int);
extern "C" void func_0020F9F0(int, int, int, int, char *);

extern "C" int func_00254D68(char *, char *, ...);

void func_0017A678(Obj *o, int delta)
{
    char buf[0x20];
    int width, height;
    int center, y, next, place, value, remaining, count, i;
    Rider *start, *r;
    int *first, *names;
    float posZ, posY, f;

    if (D_004B25A0.x10C == 18) {
        func_0017A328(o, delta);
    } else if (o->x1364 > 0) {
        if (delta > 0) {
            /* Move the x1220 vector (passed to func_00176930) down to its floor values.
               The redundant else-store and the reuse of f/posZ match the original
               register allocation. */
            posZ = o->x1228;
            posZ -= 0.1f;
            o->x1228 = posZ;
            f = 0.3173f;
            posY = o->x1224;
            posY -= f;
            o->x1224 = posY;
            if (posZ < 2.9f) o->x1228 = 2.9f;
            else o->x1224 = posY;
            f = o->x1224;
            posZ = 7.47f;
            if (f < posZ) o->x1224 = posZ;
        }
        func_0020FB80(0, 800, 576, -171, -32);
        place = o->x1164->x798C;
        if (o->x1184) {
            if (func_0023DED0() == 2) {
                func_00176930(o, o->x1184, &o->x1220, 1.0f, 1.75f, 0.62f);
            } else {
                func_00176930(o, o->x1184, &o->x1220, 1.0f, 1.0f, 1.25f);
            }
            if (o->xF98 && o->xF94 && place - 1 < o->xF94->x8) {
                o->xF98->x20 = o->xF94->x20[place - 1];
                o->xF98->x20->x4C = (float)o->x1364 * 0.005882353f;
            }
        }
        func_0023D650(o->x1280, &width, &height);
        center = width / 2;
        y = 72;
        if (func_0023DED0() == 2) y = 50;
        if (D_004B25A0.x288 >= 2) {
            func_0020FB80(0, 352, 256, -123, -32);
        } else {
            func_0020FB80(0, 384, 288, -123, -32);
        }
        func_0020F588(o->x1280, 0, 99, 104, 206, o->x1364);
        if (place == 1) {
            func_0020F9F0(0, o->x1280, center, y, D_0028B928);
        } else if (place == 2) {
            func_0020F9F0(0, o->x1280, center, y, D_0028BD20);
        } else if (place == 3) {
            func_0020F9F0(0, o->x1280, center, y, D_0028BC90);
        }
        if (D_004B25A0.x110 == 1 &&
            (D_004B25A0.x288 == 1 || D_0043EEC0.x44 >= D_004B25A0.x288)) {
            if (D_004B25A0.x288 >= 2) {
                func_0020FB80(0, 256, 192, -107, -32);
            } else {
                func_0020FB80(0, 336, 256, -123, -32);
            }
            func_0020F588(o->x1280, 0, 99, 104, 206, o->x1364);
            if (D_00381CF0[D_004B25A0.x10C].get8()) {
                if (D_00381CF0[D_004B25A0.x10C].getC()) {
                    if (D_004B25A0.x10C == 0) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BD30);
                    } else if (D_004B25A0.x10C == 1) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BD48);
                    } else if (D_004B25A0.x10C == 2) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BD60);
                    } else if (D_004B25A0.x10C == 3) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BD78);
                    } else if (D_004B25A0.x10C == 4) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BD90);
                    } else if (D_004B25A0.x10C == 5) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BDA8);
                    } else if (D_004B25A0.getMode() >= 6 && D_004B25A0.getMode() < 10) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BDC0);
                    }
                    y += 18;
                } else if (func_0014EA70(&D_00381CF0[D_004B25A0.x10C])) {
                    if (D_004B25A0.x288 == 2) {
                        value = func_00194968(&D_004225A0, D_004B25A0.x10C);
                    } else {
                        value = func_00194C48(D_004225A0.getProfile((D_004B25A0.x290 >> 5) - 1));
                    }
                    if (func_0023DED0() == 1) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BDD8);
                        next = y + 42;
                        func_0020F9F0(0, o->x1280, center, y + 30, D_0028BDE8);
                        func_0020F9F0(0, o->x1280, center, next, D_0026EFE8[value]);
                        y = next;
                    } else {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BDF8);
                        func_0020F9F0(0, o->x1280, center, y + 32, D_0026EFE8[value]);
                        y += 32;
                    }
                } else {
                    if (D_004B25A0.x288 == 2) {
                        value = func_00194968(&D_004225A0, D_004B25A0.x10C);
                    } else {
                        value = func_00194C48(D_004225A0.getProfile((D_004B25A0.x290 >> 5) - 1));
                    }
                    if (func_0023DED0() == 1) {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BE18);
                        next = y + 42;
                        func_0020F9F0(0, o->x1280, center, y + 30, D_0028BE28);
                        func_0020F9F0(0, o->x1280, center, next, D_0026EFE8[value]);
                        y = next;
                    } else {
                        func_0020F9F0(0, o->x1280, center, y + 18, D_0028BE40);
                        func_0020F9F0(0, o->x1280, center, y + 32, D_0026EFE8[value]);
                        y += 32;
                    }
                }
            } else if (func_0014EC78(&D_00381CF0[D_004B25A0.x10C])) {
                func_0020F9F0(0, o->x1280, center, y + 18, D_0028BE60);
                y += 18;
            } else if (D_004B25A0.x10C < 3 && RaceLimitReachedLegacy(&D_00381CF0[D_004B25A0.x10C])) {
                func_0020F9F0(0, o->x1280, center, y + 18, D_0028BE80);
                y += 18;
            } else {
                if (D_004B25A0.x288 >= 2) {
                    remaining = 12 - func_00194940(&D_004225A0, D_004B25A0.x10C);
                } else {
                    remaining = 12 - (D_004B25A0.x10C + D_004225A0.getProfile((D_004B25A0.x290 >> 5) - 1)->modes)->x44;
                }
                if (D_0043EEC0.x44 == D_004B25A0.x288) {
                    func_00254D68(buf, D_0028BEA0, func_00194430(&D_004225A0, place, D_004B25A0.x2A0));
                    func_0020F9F0(0, o->x1280, center, y + 18, buf);
                    if (func_0023DED0() == 1) {
                        func_00254D68(buf, D_0028BEB8, remaining);
                        next = y + 44;
                        func_0020F9F0(0, o->x1280, center, y + 32, buf);
                        func_0020F9F0(0, o->x1280, center, next, D_0028BEC8);
                        y = next;
                    } else {
                        func_00254D68(buf, D_0028BEE0, remaining);
                        func_0020F9F0(0, o->x1280, center, y + 32, buf);
                        y += 32;
                    }
                }
            }
        }
        start = D_002AF2C0.riders;
        count = D_002AF2C0.x501A8;
        first = D_0043EEC0.getNames();
        if (count > 0) {
            names = first;
            r = start;
            i = count;
            do {
                if (r == o->x1164) {
                    FormatHmsDiscard(*names, (char *)(buf));
                    if (D_004B25A0.x288 >= 2) {
                        func_0020F9F0(0, o->x1280, center, y + 15, buf);
                    } else {
                        func_0020F9F0(0, o->x1280, center, y + 24, buf);
                    }
                }
                names++;
                i--;
                r++;
            } while (i);
        }
        y += 40;
        if (D_004B25A0.x288 == 1) {
            if (o->x135F == 0 && (int)o->x1164->x661C > 40000) {
                func_0020FB80(0, 288, 224, -107, -32);
                func_0020F9F0(0, o->x1280, center, y, D_0028BF08);
                func_0020F9F0(0, o->x1280, center, y + 10, D_0028BF18);
                y += 24;
                D_004225A0.x1219C = 1;
            }
            if (o->x1360 == 0 && (int)o->x1164->x661C > 60000) {
                func_0020FB80(0, 288, 224, -107, -32);
                func_0020F9F0(0, o->x1280, center, y, D_0028BF08);
                func_0020F9F0(0, o->x1280, center, y + 10, D_0028BD08);
                D_004225A0.x1219E = 1;
            }
        }
        o->x1364 += delta;
        if (o->x1364 >= 170) o->x1364 = 170;
    }
}
