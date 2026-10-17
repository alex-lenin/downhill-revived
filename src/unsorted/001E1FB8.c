/* UNIT 0x001E1FB8 0x001E2078 */
struct Event {
    char pad0[0xC];
    short type;
    char padE[2];
};

struct EventList {
    char pad0[0x10];
    struct Event events[1];
};

extern char D_003F52D0[];
extern char D_004B28FC[];
extern char D_003F53FD[];
extern char D_00386680[];
extern char D_00387A90[];

extern void func_00190890(void *);
extern void func_00187AB0(void);
extern void func_0015C990(void);
extern void func_00190660(void *, void *);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

void func_001E1FB8(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else if (IsEvent(&list->events[index], 0x28)) {
        D_004B28FC[0] = 1;
        func_0015C990();
        D_003F53FD[0] = 1;
    } else if (IsEvent(&list->events[index], 0x29)) {
        func_00190660(D_003F52D0, D_00386680);
    } else if (IsEvent(&list->events[index], 0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    }
}
