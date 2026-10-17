/* UNIT 0x00168680 0x001687F0 */
struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x290 - 0x110];
    int x290;
    char pad294[0x2A0 - 0x294];
    int x2A0;
    char pad2A4[0x354 - 0x2A4];
    int x354;
    char pad358[4];
    char x35C;
};

struct Screen {
    char pad0[0x500];
    int x500;
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

extern struct State D_004B25A0;
extern struct Screen D_0038A2C0;
extern char D_003F53FD[];
extern char D_004B28FC[];
extern char D_003F52D0[];
extern char D_00387A90[];
extern char D_00380770[];
extern int D_003817BC[];
extern unsigned char D_00398B04[];
extern void func_00190890(void *);
extern void func_00187AB0(void);
extern void func_0015C990(void);
extern void func_00190660(void *, void *);
extern void func_001507E8(void *);
extern void func_00150E30(void *, int, int);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

static inline void HandleEvent(struct Event *e)
{
    if (IsEvent(e, 0x28)) {
        D_004B28FC[0] = 1;
        func_0015C990();
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    } else if (IsEvent(e, 0x47)) {
        D_0038A2C0.x500 = 0;
        func_00190660(D_003F52D0, &D_0038A2C0);
    } else if (IsEvent(e, 0x9A)) {
        D_004B25A0.x35C = 0;
        D_004B25A0.x354 = 0;
        D_004B25A0.x2A0 = 0x1E;
        func_001507E8(D_00380770);
        D_003817BC[0] = D_004B25A0.x10C;
        D_004B25A0.x10C = 0x14;
        func_00150E30(D_00380770, (D_004B25A0.x290 >> 5) - 1, 1);
        D_003F53FD[0] = 1;
    }
}

void func_00168680(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x1B)) {
        D_004B25A0.x35C = 0;
        if (D_00398B04[0]) {
            D_004B25A0.x354 = 1;
        }
        D_003F53FD[0] = 1;
    } else if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else {
        HandleEvent(&list->events[index]);
    }
}
