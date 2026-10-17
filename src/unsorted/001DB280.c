/* UNIT 0x001DB280 0x001DB4C0 */
#include "team_roster.h"
typedef struct Global {
    char pad0[0x288];
    int x288;
    char pad28C[0x14];
    int x2A0;
} Global;

typedef struct Sub {
    char pad0[0x20];
    int flags[4];
} Sub;

typedef struct Obj {
    char pad0[0x10];
    Sub subs[9];
} Obj;

extern Global D_004B25A0;

extern void func_001D2CB8(Sub *s);
extern void func_001D1C58(void *p);
extern int func_001D4E80(Obj *o);
extern void func_001C0B60(void *p);

void func_001DB280(Obj *obj) {
    int buf[4];
    int mode;

    func_001DB9E0(0x648, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->subs[2].flags[i] = buf[i];
            }
        }
    }
    func_001DB9E0(0x64C, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->subs[0].flags[i] = buf[i];
            }
        }
    }
    func_001DB9E0(0x64A, buf);
    {
        int i;
        for (i = 0; i < D_004B25A0.x288; i++) {
            if (buf[i] == 1) {
                obj->subs[0].flags[i] = buf[i];
                obj->subs[1].flags[i] = buf[i];
                obj->subs[2].flags[i] = buf[i];
                obj->subs[3].flags[i] = buf[i];
                obj->subs[5].flags[i] = buf[i];
                obj->subs[6].flags[i] = buf[i];
                obj->subs[7].flags[i] = buf[i];
            }
        }
    }
    func_001D2CB8(&obj->subs[0]);
    func_001D2CB8(&obj->subs[1]);
    func_001D2CB8(&obj->subs[2]);
    func_001D2CB8(&obj->subs[3]);
    func_001D2CB8(&obj->subs[4]);
    func_001D2CB8(&obj->subs[5]);
    func_001D2CB8(&obj->subs[6]);
    func_001D2CB8(&obj->subs[7]);
    func_001D2CB8(&obj->subs[8]);
    func_001D1C58((char *)obj + 0x1C0);
    func_001D1C58((char *)obj + 0x230);
    func_001D1C58((char *)obj + 0x2A0);
    mode = func_001D4E80(obj);
    if (D_004B25A0.x2A0 == 0x13) {
        func_001C0B60((char *)obj + 0x310);
    }
}
