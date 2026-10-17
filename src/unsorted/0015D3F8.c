/* UNIT 0x0015D3F8 0x0015D4E8 */
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

struct Screen {
    char pad0[0x500];
    int x500;
};

extern struct Screen D_0038A2C0;
extern char D_003F52D0[];
extern char D_00387590[];
extern void func_00190660(void *, void *);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

void func_0015D3F8(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x9F)) {
        func_00190660(D_003F52D0, D_00387590);
    } else if (IsEvent(&list->events[index], 0xA0) || IsEvent(&list->events[index], 0x38)) {
        D_0038A2C0.x500 = 0;
        func_00190660(D_003F52D0, &D_0038A2C0);
    } else if (IsEvent(&list->events[index], 0xA1)) {
        D_0038A2C0.x500 = 1;
        func_00190660(D_003F52D0, &D_0038A2C0);
    } else if (IsEvent(&list->events[index], 0xA2)) {
        D_0038A2C0.x500 = 2;
        func_00190660(D_003F52D0, &D_0038A2C0);
    } else if (IsEvent(&list->events[index], 0xA3)) {
        D_0038A2C0.x500 = 3;
        func_00190660(D_003F52D0, &D_0038A2C0);
    }
}
