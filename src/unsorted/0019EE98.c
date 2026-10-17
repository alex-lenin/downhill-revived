/* UNIT 0x0019EE98 0x0019F210 */
#include "timer.h"
typedef struct Name { int x[4]; } Name;

typedef struct Launch {
    int x0;
    float x4;
    float x8;
} Launch;

typedef struct Rider {
    char pad0[0xA0];
    Launch launch;
    char padAC[0x380 - 0xAC];
} Rider;

typedef struct World { char pad0[0x5350]; char x5350[0x88C]; int x5BDC; } World;

typedef struct GameMode {
    char pad0[0x10C];
    int mode;
    int subMode;
    char pad114[0x288 - 0x114];
    int numRiders;
    char pad28C[0x4DC - 0x28C];
    int x4DC;
} GameMode;

/* the race start sequence object (D_0043EEC0) */
typedef struct RaceStart {
    char pad0[8];
    int timer;
    char padC[4];
    int ready;
    int startTime;
    char pad18[0x180 - 0x18];
    Name name;
    char pad190[0x488 - 0x190];
    int state;
} RaceStart;

extern int func_00238F48(void);
extern int func_00238F60(void);
extern void func_0019F210(RaceStart *);
extern int func_001C2A90(char *);
extern int func_001C2EF8(char *);
extern void func_001C2AE8(char *);
extern void func_001C3080(char *);
extern void func_0019EDE8(RaceStart *);
extern void func_00148AE0(Rider *);
extern void func_00148560(Rider *, int);
extern void func_00176E40(char *, int);
extern void func_00190108(char *, char *);
extern void func_002038E0(Name *, int);
extern void func_0017BE38(char *);
extern int func_001C33D0();
extern void func_001BC4C0(char *);
extern void func_0011BFD0(char *);
extern void func_001A8A10(char *);
extern void func_0017BD80(char *, int);
extern void func_0016C2A0(World *, int);
extern void func_0011B508(char *, int);

extern char D_002AF2C0[];
extern Rider D_0037EA60[];
extern char D_00397180[];
extern World D_00398C40;
extern char D_003B2270[];
extern char D_003F52D0[];
extern char D_0043F9F0[];
extern GameMode D_004B25A0;
extern int D_004B2A7C[];
extern char D_004BD740[];
extern char D_004C4C30[];
extern char D_00759780[];

static inline World *GetWorld(void) { return &D_00398C40; }

void func_0019EE98(RaceStart *race) {
    Name name;
    int state;
    int timer;
    int i;

    if (race->state == 0)
        return;
    race->timer = race->timer + func_00238F60();
    func_0019F210(race);
    state = race->state;
    if (state == 3)
        return;
    if (state == 5) {
        func_00236C60((TimerPool *)(D_00759780));
        if (func_001C2A90(D_004BD740) != 0) {
            if (func_001C2EF8(D_004BD740) != 0)
                func_0019EDE8(race);
            else
                func_001C3080(D_004BD740);
        } else {
            func_001C2AE8(D_004BD740);
        }
    } else if (state == 1) {
        int now = func_00238F48();
        int elapsed = now - race->startTime;
        if (elapsed < 2 && race->ready != 0) {
            for (i = 0; i < D_004B25A0.numRiders; i++) {
                Rider *rider = &D_0037EA60[i];
                Launch *launch = &rider->launch;
                launch->x0 = 0;
                launch->x4 = -75.0f;
                launch->x8 = 20.0f;
                func_00148AE0(rider);
                func_00148560(rider, 8);
            }
            { char *manager = D_003B2270; func_00176E40(manager + 0x150, 1); }
        }
        if ((D_004B25A0.mode == 5 && D_004B25A0.subMode == 1) || D_004B25A0.mode == 11) {
            if (D_004B25A0.mode == 11) {
                func_00190108(D_003F52D0, D_004C4C30);
            } else {
                char *ui = D_00397180;
                *(int *)(ui + 0x500) = 1;
                func_00190108(D_003F52D0, ui);
            }
            if (*(int *)&race->name != 0) {
                name = race->name;
                func_002038E0(&name, 1);
            }
            race->state = 2;
            func_0017BE38(D_003B2270);
            D_004B2A7C[0] = 1;
            func_001C33D0();
            func_001BC4C0(GetWorld()->x5350);
            func_0011BFD0(D_002AF2C0);
            race->timer = 0;
        } else {
            timer = race->timer;
            if (timer >= 261) {
                if (*(int *)&race->name != 0) {
                    name = race->name;
                    func_002038E0(&name, 1);
                }
                race->state = 2;
                func_0017BE38(D_003B2270);
                D_004B25A0.x4DC = 1;
                func_001C33D0();
                func_001BC4C0(GetWorld()->x5350);
                func_0011BFD0(D_002AF2C0);
                func_001A8A10(D_0043F9F0);
                race->timer = 0;
            } else {
                func_0017BD80(D_003B2270, 300 - timer);
                func_0016C2A0(GetWorld(), GetWorld()->x5BDC);
            }
        }
    } else if (state == 2) {
        if (race->timer >= 481) {
            race->state = 3;
            func_0011B508(D_002AF2C0, 1);
        }
    }
}
