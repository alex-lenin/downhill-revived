/* UNIT 0x0014EFA0 0x0014F1F8 */
struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x2A0 - 0x110];
    int x2A0;
    char pad2A4[0x354 - 0x2A4];
    int x354;
    char pad358[0x35C - 0x358];
    char x35C;
};

/* 16-byte event entry; type at +0xC */
struct Event {
    char pad0[0xC];
    short type;
    char padE[2];
};

struct EventList {
    char pad0[0x10];
    struct Event events[1];
};

typedef struct {
    char pad[0x12D];
    char x12D;
} MenuState;

typedef struct {
    char pad[0x796C];
    int x796C;
} Rider;

typedef struct {
    Rider *rider;
    char pad4[0x14 - 4];
} Slot;

extern struct State D_004B25A0;
extern char D_004B28FC[];
extern MenuState D_003F52D0;
extern char D_003F53FD[];
extern int D_003817BC[];
extern char D_00380770[];
extern char D_0038BC00[];
extern void *D_0038C600[];
extern char D_0038B1F0[];
extern char D_00380270[];
extern char D_00387A90[];
extern Slot D_0043F870[];

extern void func_001507E8(void *obj);
extern void func_00150E30(void *obj, int val, int idx);
extern void func_0015C920(void);
extern void func_0015C990(void);
extern void func_00187AB0(void);
extern void func_00190890(void *menu);
extern void func_00190660(void *menu, void *res);
#include "scene_effects.h"
extern void func_00139BF8(int flag);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

static inline void HandleEvent(struct Event *e)
{
    if (IsEvent(e, 0x1D)) {
        func_0015C920();
        D_0038C600[0] = D_0038BC00;
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x1F)) {
        D_004B28FC[0] = 0;
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x20)) {
        D_004B28FC[0] = 1;
        func_0015C990();
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x30)) {
        func_00190660(&D_003F52D0, D_0038B1F0);
    } else if (IsEvent(e, 0x3A)) {
        func_00190660(&D_003F52D0, D_00380270);
    } else if (IsEvent(e, 0x1A)) {
        func_00190660(&D_003F52D0, D_00387A90);
    } else if (IsEvent(e, 0x24)) {
        func_0020ECD0(0x25, 0x10, 0xF, 370.0f, 15120.0f, 255.0f, 16380.0f);
        func_00139BF8(1);
        D_004B28FC[0] = 0;
    }
}

void func_0014EFA0(struct EventList *list, int index)
{
    int m;
    int m2;
    int i;
    Slot *e;
    struct State *s;

    if (IsEvent(&list->events[index], 0x9A)) {
        s = &D_004B25A0;
        m = s->x10C;
        s->x35C = 0;
        s->x354 = 0;
        if (m == 3)
            s->x2A0 = 0x1D;
        else if (m == 4)
            s->x2A0 = 0x1F;
        else
            s->x2A0 = 0x1E;
        m2 = D_004B25A0.x10C;
        D_003817BC[0] = m2;
        D_004B25A0.x10C = 0x14;
        func_001507E8(D_00380770);
        i = 0;
        e = D_0043F870;
        for (; i < 5; i++) {
            func_00150E30(D_00380770, (e->rider->x796C >> 5) - 1, i);
            e++;
        }
        D_003F52D0.x12D = 1;
    } else if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(&D_003F52D0);
        func_00187AB0();
    } else {
        HandleEvent(&list->events[index]);
    }
}
