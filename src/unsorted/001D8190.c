/* UNIT 0x001D8190 0x001D8618 */
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
    Sub40 x10[16];
    Sub30 x410[14];
    char x6B0[0x20];
    Sub30 x6D0;
    Sub70 x700[4];
    char x8C0[0x10];
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
extern void func_001C0E00(void *);
extern void func_001D4E80(Obj *);

void func_001D8190(Obj *obj)
{
    int buf[4];

    if (D_004B25A0.x2A0 == 4) {
        func_001DB510(&obj->x10[0]);
        func_001DB510(&obj->x10[1]);
        func_001DB510(&obj->x10[2]);
        func_001DB510(&obj->x10[3]);
        func_001DB510(&obj->x10[4]);
        func_001DB510(&obj->x10[5]);
        func_001DB510(&obj->x10[6]);
        func_001DB510(&obj->x10[7]);
        func_001DB510(&obj->x10[7]);
        func_001DB510(&obj->x10[9]);
        func_001DB510(&obj->x10[10]);
        func_001DB510(&obj->x10[11]);
    } else if (D_004B25A0.x2A0 == 3) {
        func_001DB510(&obj->x10[12]);
        func_001DB510(&obj->x10[13]);
        func_001DB510(&obj->x10[14]);
        func_001DB510(&obj->x10[15]);
        func_001C0E00(obj->x6B0);
    } else if (D_004B25A0.x2A0 == 9) {
        func_001DB510(&obj->x10[12]);
        func_001DB510(&obj->x10[13]);
        func_001DB510(&obj->x10[14]);
        func_001DB510(&obj->x10[15]);
    }

    func_001DB9E0(0x652, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[0].flags[i] = buf[i];
                obj->x410[1].flags[i] = buf[i];
                obj->x410[2].flags[i] = buf[i];
                obj->x410[3].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x650, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[0].flags[i] = buf[i];
                obj->x410[1].flags[i] = buf[i];
                obj->x410[2].flags[i] = buf[i];
                obj->x410[3].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x645, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[0].flags[i] = buf[i];
                obj->x410[1].flags[i] = buf[i];
                obj->x410[2].flags[i] = buf[i];
                obj->x410[3].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x648, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[2].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x64C, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[0].flags[i] = buf[i];
            }
        }
    }

    func_001DB9E0(0x645, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->x410[0].flags[i] = buf[i];
                obj->x410[1].flags[i] = buf[i];
                obj->x410[2].flags[i] = buf[i];
                obj->x410[3].flags[i] = buf[i];
                obj->x410[4].flags[i] = buf[i];
                obj->x410[6].flags[i] = buf[i];
                obj->x410[12].flags[i] = buf[i];
                obj->x410[8].flags[i] = buf[i];
                obj->x410[9].flags[i] = buf[i];
                obj->x410[10].flags[i] = buf[i];
            }
        }
    }

    func_001D2CB8(&obj->x410[0]);
    func_001D2CB8(&obj->x410[1]);
    func_001D2CB8(&obj->x410[2]);
    func_001D2CB8(&obj->x410[3]);
    func_001D2CB8(&obj->x410[4]);
    func_001D2CB8(&obj->x410[6]);
    func_001D2CB8(&obj->x410[7]);
    func_001D2CB8(&obj->x410[12]);
    func_001D2CB8(&obj->x410[8]);
    func_001D2CB8(&obj->x410[9]);
    func_001D2CB8(&obj->x410[10]);
    func_001D2CB8(&obj->x6D0);

    func_001D1C58(&obj->x700[0]);
    func_001D1C58(&obj->x700[1]);
    func_001D1C58(&obj->x700[2]);

    if (D_004B25A0.x2A0 == 3) {
        func_001D4E80(obj);
    }
    if (D_004B25A0.x2A0 != 9) {
        func_001C0B60(obj->x8C0);
    }
}
