/* UNIT 0x001C2AE8 0x001C2EF8 .rodata=0x28FC10:0x28FC30 */
#include "race_limit.h"
typedef struct State {
    char pad0[0x10C]; int x10C;
    char pad110[0x178]; int x288;
    char pad28C[0x14]; int x2A0;
    char pad2A4[0xB4]; int x358;
} State;
typedef struct Entry { char pad[0x18]; } Entry;
typedef struct Racer { char pad[0x796C]; int x796C; } Racer;
typedef struct Game { char pad[0x50164]; Racer *riders[1]; } Game;
typedef struct Obj {
    char pad0[0xC0]; float xC0;
    char padC4[0x14]; int xD8, xDC, xE0;
    char padE4[8]; int xEC;
    char padF0[4]; float xF4;
    char padF8[4]; int xFC;
} Obj;

extern State D_004B25A0;
extern char *D_0026EFE8[];
extern Entry D_00381CF0[];
extern Game D_002AF2C0;
extern void func_0023D650(int, int *, int *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020FA98(int, int, int, int, char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00182290(int);
extern void func_001C30A8(Obj *, int, float *);

extern char *func_00158668(int);
extern int func_00254D68(char *, const char *, ...);

void func_001C2AE8(Obj *obj) {
    char buf[0x30];
    int width, height;
    int count, i;
    int freestyle; /* mode 5 = freestyle; a variable is needed to match */

    func_0023D650(0, &width, &height);
    func_0020F518(0, 0xDD, 0xC8, 10, 0xAA);
    func_0020FB80(0, 0x140, 0x120, -0x5B, -0x20);
    if (obj->xFC != 0 && D_004B25A0.x10C != 5) {
        obj->xC0 += obj->xF4;
        if (obj->xC0 > 1.4f) {
            obj->xC0 = 1.4f;
            obj->xD8 += 5;
            if (obj->xD8 > width - 20) obj->xD8 = width - 20;
            if (func_00182290(D_004B25A0.x2A0) == 2) {
                func_0020F518(0, 0x96, 0x96, 0xC8, 0xAA);
            } else if (func_00182290(D_004B25A0.x2A0) == 0) {
                func_0020F518(0, 0xFF, 0x96, 0x24, 0xAA);
            } else if (func_00182290(D_004B25A0.x2A0) == 1) {
                func_0020F518(0, 0x52, 0x36, 0xB4, 0xF0);
            }
            func_0020FA98(0, 0, obj->xD8, 0x32, D_0026EFE8[D_004B25A0.x2A0]);
        }
        func_001C30A8(obj, obj->xFC, &obj->xC0);
    } else {
        freestyle = 5;
        if (D_004B25A0.x10C == freestyle) func_0020F518(0, 0x64, 0xB3, 0x64, 0x82);
        obj->xD8 += 5;
        if (obj->xD8 > width - 20) obj->xD8 = width - 20;
        func_0020FA98(0, 0, obj->xD8, 0x20, D_0026EFE8[D_004B25A0.x2A0]);
        if (D_004B25A0.x10C == freestyle) {
            func_0020F518(0, 0, 0xB3, 0x33, 0x82);
            func_0020FB80(0, 0x226, 0x15E, -0xB4, 0);
            func_0020FA98(0, 0, obj->xD8, 0xC, "FreeStyle");
        }
    }
    if (D_004B25A0.x10C < 3 && RaceLimitReachedLegacy(&D_00381CF0[D_004B25A0.x10C])) {
        func_0020FB80(0, 0x120, 0xE0, -0x5B, -0x20);
        func_0020F518(0, 0x6E, 0x6E, 0xD2, obj->xEC);
        if (obj->xD8 >= width - 20) {
            obj->xDC += 3;
            count = D_004B25A0.x288 + D_004B25A0.x358;
            for (i = 0; i < count; i++) {
                if (i <= obj->xE0) {
                    func_00254D68(buf, "Lane %d %s", i + 1,
                                  func_00158668((D_002AF2C0.riders[i]->x796C >> 5) - 1));
                    func_0020F9A8(0, 0, i == obj->xE0 ? obj->xDC : i * 30 + 40, i * 12 + 0x46, buf);
                }
            }
            if (obj->xE0 * 30 + 40 < obj->xDC) {
                obj->xDC = 0;
                obj->xE0++;
            }
            if (obj->xE0 >= count) {
                if (--obj->xEC <= 0) obj->xEC = 0;
            }
        }
    }
}
