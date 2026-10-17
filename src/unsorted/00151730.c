/* UNIT 0x00151730 0x001517A0 */
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
extern char D_003F52D0[];
extern void func_00190890(void *);
extern void func_00187AB0(void);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

/* Each test takes &list->events[index] afresh, so the type field is reloaded. */
void func_00151730(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x1B)) {
        int on = 1;

        D_003F53FD[0] = on;
        D_004B25A0.x354 = on;
        D_004B25A0.x35C = 0;
    } else if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    }
}
