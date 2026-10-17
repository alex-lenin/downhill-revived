/* UNIT 0x0016B8C8 0x0016BC30 */
#include "timer.h"
typedef struct { char pad0[32]; int x20; char pad24[32]; unsigned char x44; } Manager;
typedef struct { char pad0[9]; unsigned char x9; } Reset;
typedef struct { char pad0[0x288]; int x288; } State;
typedef struct { char pad0[0xC80]; signed char free[32]; unsigned char count; } Pool;
typedef struct { char pad0[8]; short delta,padA; void (*method)(void *,void *); } VTable;
class Virtual { public: int x0; virtual void Method(); };
class Other { public: void func_001635E8(); };
typedef struct { int x0,x4; char pad8[0x5BF0]; Virtual *x5BF8; } Obj;
void func_0010A200();                                  /* extern */
void func_00119E60(void *);                               /* extern */
void func_001274F0(void *);                               /* extern */
void func_00143BF8(void *);                               /* extern */
void func_00146F78(void *);                               /* extern */
void func_001475D8(void *);                               /* extern */
void func_00147B50();                                  /* extern */
void func_001481E8(void *);                               /* extern */
void func_00151DB0();                                  /* extern */
void func_00151F68();                                  /* extern */
void func_00158058(void *);                               /* extern */
void func_001613E8(void *);                               /* extern */
void func_00162CA0();                                  /* extern */
void func_00162DB0();                                  /* extern */
                               /* extern */
void func_00164270(void *);                               /* extern */
void func_00175148(void *);                               /* extern */
void func_0017C960(void *);                               /* extern */
void func_0017D388();                                  /* extern */
void func_00183368(void *);                               /* extern */
void func_0018FF48(void *);                               /* extern */
void func_00192DD8(signed char *);                              /* extern */
void func_001936C0(void *, int);                            /* extern */
void func_00196038(void *);                               /* extern */
void func_00199030(void *, int);                            /* extern */
void func_0019C200(void *);                               /* extern */
void func_0019E950(void *);                               /* extern */
void func_001A8958(void *);                               /* extern */
void func_001AA140();                                  /* extern */
void func_001AB8C8(void *, int);                            /* extern */
void func_001C27F8(void *);                               /* extern */
void func_001DC240(void *);                               /* extern */
void func_001E5040();                                  /* extern */
void func_001EAC40(void *);                               /* extern */
void func_00210F20(int);                                 /* extern */
void func_0021D7E8();                                  /* extern */
void func_0021E428(int);                                 /* extern */
void func_0021E438(float);                                 /* extern */
void func_00220D88();                                  /* extern */
void func_00222E10(int, int);                            /* extern */
void func_00228BE8(int);                                 /* extern */
int func_00238F10(void);                               /* extern */
void func_0023C8D8(int, int);                              /* extern */
void func_0023D610(int);                                 /* extern */
void func_0023D898(int, float *, float *);                  /* extern */
void func_0023D8D8(int, float, float);                         /* extern */
extern Other D_0029DD70;
extern char D_002AF2C0[];
extern char D_00308D40[];
extern char D_0036B870[];
extern char D_00372650[];
extern char D_00373630[];
extern char D_0037EA60[];
extern char D_00383A10[];
extern char D_0038DB60[];
extern char D_0038DCE0[];
extern char D_003B2270[];
extern char D_003B71D0[];
extern char D_003C6930[];
extern char D_003F52D0[];
extern char D_00412510[];
extern char D_004348F0[];
extern char D_00438750[];
extern Reset D_0043DD38;
extern char D_0043EEC0[];
extern Manager D_0043F9F0;
extern char D_004A6390[];
extern Pool D_004A9160;
extern State D_004B25A0;
extern char D_004BD740[];
extern char D_004BF950[];
extern char D_004C5C50[];
extern char D_00759780[];
void func_0016B8C8(Obj *arg0) {
    float sp0,sp4;
    int temp_v0;
    signed char *var_a0; Pool *pool;
    int var_v1;
    Virtual *temp_a1;
    VTable *temp_v0_2;

    arg0->x4 = 1;
    if (D_0043F9F0.x44 != 0) {
        func_00222E10(D_0043F9F0.x20, 1);
    } else {
        temp_v0 = func_00238F10();
        func_00222E10(temp_v0, 0);
        func_001A8958(&D_0043F9F0);
        D_0043F9F0.x20 = temp_v0;
        D_0043DD38.x9 = 0;
    }
    func_00236A30((TimerPool *)(&D_00759780), 0x64);
    func_0010A200();
    func_00199030(&D_00438750, 0);
    func_001AB8C8(&D_004A6390, 0);
    func_00162CA0();
    func_00162DB0();
    func_001274F0(&D_00308D40);
    func_001936C0(&D_00412510, 0);
    pool=&D_004A9160;
    var_v1 = 0x1F;
    var_a0 = &pool->free[31];
    do {
        *var_a0 = var_v1;
        var_v1 -= 1;
        var_a0 -= 1;
    } while (var_v1 >= 0);
    pool->count = 0;
    func_00192DD8(var_a0);
    func_00164270(&D_0038DCE0);
    func_0019C200(&D_0043DD38);
    if(D_004B25A0.x288==1) {
        func_0023D610(1);
        func_0023C8D8(0, 0);
        func_0023D898(0, &sp0, &sp4);
        func_0023D8D8(0, 277.0f, sp4);
    } else if(D_004B25A0.x288==2) {
        func_0023D610(2);
        func_0023C8D8(3, 0);
        func_0023C8D8(4, 1);
    } else if(D_004B25A0.x288>=3) {
        func_0023D610(4);
        func_0023C8D8(6, 0);
        func_0023C8D8(7, 1);
        func_0023C8D8(8, 2);
        func_0023C8D8(9, 3);
    }
    func_00228BE8(1);
    func_0017D388();
    func_00146F78(&D_00372650);
    func_001475D8(&D_00373630);
    func_0017C960(&D_003B71D0);
    func_001DC240(&D_004BF950);
    func_00147B50();
    func_001EAC40(&D_004C5C50);
    func_00151DB0();
    func_00151F68();
    func_00119E60(&D_002AF2C0);
    func_001481E8(&D_0037EA60);
    D_0029DD70.func_001635E8();
    func_00143BF8(&D_0036B870);
    func_00175148(&D_003B2270);
    func_001E5040();
    func_00158058(&D_00383A10);
    func_0019E950(&D_0043EEC0);
    func_00196038(&D_004348F0);
    func_0018FF48(&D_003F52D0);
    temp_a1 = arg0->x5BF8;
    if (temp_a1 != 0) {
        temp_a1->Method();
    }
    func_0021E438(6.5f);
    func_0021D7E8();
    func_0021E428(1);
    func_00210F20(0);
    func_001AA140();
    func_00220D88();
    func_001613E8(&D_0038DB60);
    func_00183368(&D_003C6930);
    func_001C27F8(&D_004BD740);
}
