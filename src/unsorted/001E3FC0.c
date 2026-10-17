/* UNIT 0x001E3FC0 0x001E4110 */
typedef struct State {
    char pad0[0x500];
    int x500;
    char pad504[0x519 - 0x504];
    unsigned char x519;
} State;

typedef struct Flags {
    char pad0[0x354];
    int x354;
    char pad358[4];
    char x35C;
} Flags;

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

extern State D_004C5640;
extern State D_0038A2C0;
extern Flags D_004B25A0;
extern char D_003F53FD[];
extern char D_004B28FC[];
extern char D_003F52D0[];
extern char D_00387A90[];
extern char D_004C5130[];
extern void func_00190890(void *);
extern void func_00187AB0(void);
extern void func_0015C990(void);
extern void func_00190660(void *, void *);
extern void func_001E4120(State *);

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
    } else if (IsEvent(e, 0x98)) {
        func_00190660(D_003F52D0, D_004C5130);
    }
}

void func_001E3FC0(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x1B)) {
        if (D_004C5640.x519) {
            D_004B25A0.x354 = 1;
            D_004B25A0.x35C = 0;
        } else {
            D_004B28FC[0] = 1;
            func_0015C990();
            D_004C5640.x500++;
            func_001E4120(&D_004C5640);
        }
        D_003F53FD[0] = 1;
    } else if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else {
        HandleEvent(&list->events[index]);
    }
}
