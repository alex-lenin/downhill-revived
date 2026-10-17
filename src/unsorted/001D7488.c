/* UNIT 0x001D7488 0x001D7838 */
#include "team_roster.h"
typedef struct {
    char pad[0x30];
    int flags[4];
} Sub40;

typedef struct {
    char pad[0x20];
    int flags[4];
} Sub30;

typedef struct {
    char pad[0x70];
} Sub70;

typedef struct {
    char pad[0x10];
    Sub40 x10[8];
    Sub30 x210[11];
    Sub70 x420[4];
    Sub40 x5e0[3];
} Obj;

typedef struct {
    char pad[0x288];
    int x288;
    char pad2[0x14];
    int x2A0;
} Global;

extern Global D_004B25A0;


extern void func_001DB510(void *);
extern void func_001D2CB8(void *);
extern void func_001D1C58(void *);
extern void func_001C0B60(void *);

void func_001D7488(Obj *obj)
{
    int buf[4];

    func_001DB9E0(0x648, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x210[3].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x64C, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x210[1].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x645, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x210[1].flags[i] = buf[i];
                obj->x210[2].flags[i] = buf[i];
                obj->x210[6].flags[i] = buf[i];
                obj->x210[7].flags[i] = buf[i];
                obj->x210[8].flags[i] = buf[i];
                obj->x210[3].flags[i] = buf[i];
                obj->x210[4].flags[i] = buf[i];
                obj->x210[0].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x64A, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x210[1].flags[i] = buf[i];
                obj->x210[2].flags[i] = buf[i];
                obj->x210[6].flags[i] = buf[i];
                obj->x210[7].flags[i] = buf[i];
                obj->x210[8].flags[i] = buf[i];
                obj->x210[3].flags[i] = buf[i];
                obj->x210[4].flags[i] = buf[i];
                obj->x210[0].flags[i] = buf[i];
            }
        }
    }

    func_001DB510(&obj->x10[0]);
    func_001DB510(&obj->x10[1]);
    func_001DB510(&obj->x10[2]);
    func_001DB510(&obj->x10[3]);
    func_001DB510(&obj->x10[4]);
    func_001DB510(&obj->x10[5]);
    func_001DB510(&obj->x10[6]);
    func_001DB510(&obj->x10[7]);

    func_001D2CB8(&obj->x210[1]);
    func_001D2CB8(&obj->x210[2]);
    func_001D2CB8(&obj->x210[6]);
    func_001D2CB8(&obj->x210[7]);
    func_001D2CB8(&obj->x210[5]);
    func_001D2CB8(&obj->x210[8]);
    func_001D2CB8(&obj->x210[10]);
    func_001D2CB8(&obj->x210[3]);
    func_001D2CB8(&obj->x210[4]);
    func_001D2CB8(&obj->x210[0]);
    func_001D2CB8(&obj->x210[9]);

    func_001D1C58(&obj->x420[0]);
    func_001D1C58(&obj->x420[1]);
    func_001D1C58(&obj->x420[2]);
    func_001D1C58(&obj->x420[3]);

    {
        int mode = D_004B25A0.x2A0;
        if (mode == 10) {
            func_001DB510(&obj->x5e0[0]);
            func_001DB510(&obj->x5e0[1]);
            func_001DB510(&obj->x5e0[2]);
        } else if (mode == 2) {
            func_001DB510(&obj->x5e0[2]);
        } else if (mode == 8) {
            func_001DB510(&obj->x5e0[2]);
        }
    }

    if (D_004B25A0.x2A0 != 8) {
        func_001C0B60((char *)obj + 0x9D0);
    }
}
