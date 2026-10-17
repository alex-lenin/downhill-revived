/* UNIT 0x001D58F0 0x001D5C30 */
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
    char pad0[0x10];
    Sub40 x10[4];
    Sub30 x110[9];
    Sub70 x2C0[3];
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

void func_001D58F0(Obj *obj)
{
    int buf[4];

    if (D_004B25A0.x2A0 == 1) {
        func_001DB510(&obj->x10[0]);
        func_001DB510(&obj->x10[1]);
        func_001DB510(&obj->x10[2]);
    } else if (D_004B25A0.x2A0 == 0xD) {
        func_001DB510(&obj->x10[1]);
    }

    func_001DB9E0(0x645, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x110[0].flags[i] = buf[i];
                obj->x110[1].flags[i] = buf[i];
                obj->x110[5].flags[i] = buf[i];
                obj->x110[6].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x648, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x110[5].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x64C, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x110[0].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x645, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x110[5].flags[i] = buf[i];
                obj->x110[6].flags[i] = buf[i];
                obj->x110[7].flags[i] = buf[i];
                obj->x110[0].flags[i] = buf[i];
                obj->x110[1].flags[i] = buf[i];
                obj->x110[2].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x64A, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x110[5].flags[i] = buf[i];
                obj->x110[6].flags[i] = buf[i];
                obj->x110[7].flags[i] = buf[i];
                obj->x110[0].flags[i] = buf[i];
                obj->x110[1].flags[i] = buf[i];
                obj->x110[2].flags[i] = buf[i];
            }
        }
    }

    func_001D2CB8(&obj->x110[3]);
    func_001D2CB8(&obj->x110[4]);
    func_001D2CB8(&obj->x110[5]);
    func_001D2CB8(&obj->x110[6]);
    func_001D2CB8(&obj->x110[7]);
    func_001D2CB8(&obj->x110[0]);
    func_001D2CB8(&obj->x110[1]);
    func_001D2CB8(&obj->x110[2]);
    func_001D2CB8(&obj->x110[8]);

    func_001D1C58(&obj->x2C0[0]);
    func_001D1C58(&obj->x2C0[1]);
    func_001D1C58(&obj->x2C0[2]);

    if (D_004B25A0.x2A0 != 7) {
        func_001C0B60((char *)obj + 0x410);
    }
}
