/* UNIT 0x001682A0 0x00168680 */
#include "format.h"
struct Obj { char pad[0x500]; int x500; int x504; };
struct G1 { char pad[0x20]; int x20; };
struct Career {
    char pad[0x524];
    int x524;
    int x528;
    char pad52C[4];
    int x530;
    char pad534[0x1F];
    unsigned char x553;
    unsigned char x554;
};
struct G4 { char pad[0x12208]; int x12208; };

extern struct G1 D_003F52D0;
extern struct Career D_003985B0;
extern struct G4 D_004225A0;
extern int D_00397180[];
extern char D_0028AE00[], D_0028AE18[], D_0028AE38[], D_0028AE58[], D_0028AE68[];
extern char D_0028AE80[], D_0028AE90[], D_0029C828[], D_0029C830[];

extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F588(int, int, int, int, int, int);
extern void func_0020F9F0(int, int, int, int, const char *);
extern void func_0020F9A8(int, int, int, int, const char *);
extern void func_00167BC0(int *, int, int, int, int, int);

extern int func_00254D68(char *, const char *, ...);
extern void func_0018DC20(struct Obj *);

void func_001682A0(struct Obj *p) {
    char buf[48];
    int width, height, handle, half;

    handle = D_003F52D0.x20;
    func_0023D650(handle, &width, &height);
    half = width / 2;
    if (p->x500 < 190) {
        p->x500 += 3;
    } else {
        p->x500 = 0;
        p->x504++;
    }
    func_0020F588(handle, 0, 221, 219, 10, p->x504 == 0 ? p->x500 : 190);
    func_0020FB80(0, 384, 288, -139, -32);
    if (D_003985B0.x553) {
        if (!D_003985B0.x554) {
            func_0020F9F0(0, handle, half, 10, D_0028AE00);
            func_0020F9F0(0, handle, half, 24, D_0028AE18);
        } else {
            func_0020F9F0(0, handle, half, 10, D_0028AE00);
            func_0020F9F0(0, handle, half, 24, D_0028AE38);
        }
    } else {
        func_0020F9F0(0, handle, half, 10, D_0028AE58);
        func_0020F9F0(0, handle, half, 24, D_0028AE68);
    }
    func_0020F588(handle, 0, 99, 104, 206, p->x504 == 0 ? p->x500 : 190);
    if (p->x504 > 0) {
        int *list;
        int other;
        list = D_00397180;
        other = D_003F52D0.x20;
        func_00167BC0(list, other, 50, 0, p->x504 == 1 ? p->x500 : 190, D_003985B0.x530);
    }
    if (p->x504 >= 2) {
        if (D_003985B0.x553) {
            func_0020FB80(0, 304, 224, -123, -32);
            func_0020F518(0, 110, 110, 200, p->x504 == 2 ? p->x500 : 190);
            func_0020F9A8(0, handle, 180, 145, D_0029C828);
            FormatHmsDiscard(D_003985B0.x524, (char *)(buf));
            func_0020F518(0, 88, 152, 73, p->x504 == 2 ? p->x500 : 190);
            func_0020F9A8(0, handle, 240, 145, buf);
            if (D_003985B0.x524 < D_003985B0.x528) {
                func_0020F518(0, 221, 219, 10, p->x504 == 2 ? p->x500 : 190);
                func_0020F9F0(0, handle, half - (width >> 2) - 30, 165, D_0029C830);
                func_0020F9F0(0, handle, half - (width >> 2) - 30, 175, D_0028AE80);
            }
        }
        func_0020FB80(0, 280, 256, -98, -32);
        func_0020F518(0, 221, 219, 10, p->x504 == 2 ? p->x500 : 190);
        func_00254D68(buf, D_0028AE90, D_004225A0.x12208 / 20);
        func_0020F9A8(0, D_003F52D0.x20, 350, 170, buf);
    }
    func_0018DC20(p);
}
