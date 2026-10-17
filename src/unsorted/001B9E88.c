/* UNIT 0x001B9E88 0x001B9F90 */
#include "timer.h"
typedef struct Obj {
    char p0[0x4f0];
    char x4F0[0xaca0];
    int xB190;
    int xB194;
    int xB198;
    int xB19C;
} Obj;

extern char D_00663B08[];
extern char D_00759780[];
extern char D_0043DD38[];
extern char D_003F52D0[];
extern char D_0037EA60[];
extern void func_00223B00(char *);
extern void func_00218038(void);
extern void func_0020E860(void);
extern void func_0023D6C8(void);
extern void func_0023D610(int);
extern void func_0023C8D8(int, int);
extern void func_0023D898(int, float *, float *);
extern void func_0023D8D8(int, float, float);
extern void func_00228BE8(int);
extern void func_0019C200(char *);
extern void func_0021D7E8(void);
extern void func_0022F8C0(void);
extern void func_0018FF48(char *);
extern void func_001481E8(char *);
extern int func_00238F10(void);
extern void func_00222E10(int, int);
extern void func_001C4E68(char *);
extern void func_0013AF80(void);

void func_001B9E88(Obj *o) {
    float a;
    float b;

    func_00223B00(D_00663B08);
    func_00218038();
    func_0020E860();
    func_00236A30((TimerPool *)(D_00759780), 100);
    func_0023D6C8();
    func_0023D610(1);
    func_0023C8D8(5, 0);
    func_0023D898(0, &a, &b);
    func_0023D8D8(0, 688.0f, b);
    func_00228BE8(1);
    func_0019C200(D_0043DD38);
    func_0021D7E8();
    func_0022F8C0();
    o->xB190 = 0;
    o->xB194 = 0;
    o->xB198 = 0;
    o->xB19C = 0;
    func_0018FF48(D_003F52D0);
    func_001481E8(D_0037EA60);
    func_00222E10(func_00238F10(), 0);
    func_001C4E68(o->x4F0);
    func_0013AF80();
}
