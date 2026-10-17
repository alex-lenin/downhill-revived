/* UNIT 0x001E2710 0x001E2958 */
/* 16-byte event entry; type at +0xC */
struct Event {
    char pad0[0xC];
    short type;
    char padE[2];
};

struct Obj {
    char pad0[0x10];
    struct Event events[0x4F];
    char pad500[0x4];
    int x504[6];
    int x51c;
    unsigned char x520[4][13];
    unsigned char x554[4];
};

struct Game {
    char pad0[0x10C];
    int x10c;
    char pad110[0x178];
    int x288;
    int x28c;
    int x290;
};

struct Manager {
    char pad0[0x12D];
    unsigned char x12d;
};

extern char D_004225A0[];
extern struct Game D_004B25A0;
extern struct Manager D_003F52D0;
extern int D_0029E4B8;

extern int func_00194C80(void *, int);
extern int func_00194A10(void *, int);
extern void func_00182198(unsigned char);
extern void func_00194538(void *, int);
extern void func_001E2E28(struct Obj *);
extern void func_00190890(struct Manager *);
extern void func_00187AB0(void);
extern void func_00190688(struct Manager *);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

void func_001E2710(struct Obj *obj, int index)
{
    int which, allowed, i;

    which = index + 6;
    if (index < 4) {
        if (D_004B25A0.x288 == 1) {
            allowed = func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, which) < obj->x554[index];
            if (allowed && obj->x51c) {
                if (func_00194C80(D_004225A0 + ((D_004B25A0.x290 >> 5) - 1) * 0x694, which) <= 0)
                    allowed = 0;
            }
        } else {
            allowed = func_00194A10(D_004225A0, which) < obj->x554[index];
            if (allowed && obj->x51c) {
                if (func_00194A10(D_004225A0, which) <= 0)
                    allowed = 0;
            }
        }
        if (obj->x554[index] && allowed) {
            D_0029E4B8 = 0;
            i = 0;
            while (obj->x520[index][i] != 0x21) {
                func_00182198(obj->x520[index][i]);
                i++;
            }
            if (obj->x504[index])
                func_00194538(D_004225A0, which);
            D_003F52D0.x12d = 1;
            D_004B25A0.x10c = which;
        }
    } else if (IsEvent(&obj->events[index], 0x23)) {
        func_001E2E28(obj);
        func_00190890(&D_003F52D0);
        func_00187AB0();
    } else if (IsEvent(&obj->events[index], 0x1A)) {
        func_00190688(&D_003F52D0);
    }
}
