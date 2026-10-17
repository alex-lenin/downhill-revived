/* UNIT 0x001841A8 0x001842C8 */
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

extern int D_004B2828[];
extern char D_003F53FD[];
extern char D_004B28FC[];
extern char D_003F52D0[];
extern char D_003C6930[];
extern char D_0038A2C0[];
extern char D_0038B1F0[];
extern char D_00387A90[];
extern void func_00190890(void *);
extern void func_00187AB0(void);
extern void func_0015C990(void);
extern void func_00190660(void *, void *);
extern void func_001834C0(void *);
extern void func_001832B0(void *);

static inline int IsEvent(struct Event *e, int type)
{
    return e->type == type;
}

static inline void HandleEvent(struct Event *e)
{
    if (IsEvent(e, 0x47)) {
        if (D_004B2828[0] == 1) {
            func_00190660(D_003F52D0, D_0038A2C0);
        } else {
            func_00190660(D_003F52D0, D_0038B1F0);
        }
    } else if (IsEvent(e, 0x94)) {
        func_001832B0(D_003C6930);
        D_004B28FC[0] = 1;
        func_0015C990();
        D_003F53FD[0] = 1;
    } else if (IsEvent(e, 0x1A)) {
        func_00190660(D_003F52D0, D_00387A90);
    }
}

void func_001841A8(struct EventList *list, int index)
{
    if (IsEvent(&list->events[index], 0x23)) {
        func_00190890(D_003F52D0);
        func_00187AB0();
    } else if (IsEvent(&list->events[index], 0x93)) {
        D_004B28FC[0] = 1;
        func_001834C0(D_003C6930);
        func_0015C990();
        D_003F53FD[0] = 1;
    } else {
        HandleEvent(&list->events[index]);
    }
}
