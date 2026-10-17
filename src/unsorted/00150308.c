/* UNIT 0x00150308 0x00150410 */
struct State {
    char pad0[0x354];
    int x354;
    char pad358[4];
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

extern struct State D_004B25A0;
extern char D_003F53FD[];
extern char D_004B28FC[];
extern char D_003F52D0[];
extern char D_0038B1F0[];
extern char D_00387A90[];
extern void func_00190890(void *);
extern void func_00187AB0(void);
extern void func_0015C990(void);
extern void func_00190660(void *, void *);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

static inline void HandleEvent(struct Event *e)
{
    if (IsEvent(e, 0x1F)) {
        D_004B28FC[0] = 0;
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x20)) {
        D_004B28FC[0] = 1;
        func_0015C990();
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x30)) {
        func_00190660(D_003F52D0, D_0038B1F0);
    } else if (IsEvent(e, 0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    }
}

void func_00150308(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x1B)) {
        int on = 1;

        D_003F53FD[0] = on;
        D_004B25A0.x354 = on;
        D_004B25A0.x35C = 0;
    } else if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else {
        HandleEvent(&list->events[index]);
    }
}
