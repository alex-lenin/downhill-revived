/* UNIT 0x001414A0 0x001418F8 */
typedef struct Name {
    int x0, x4, x8;
} __attribute__((packed, aligned(4))) Name;

typedef struct State {
    Name x0;
    Name xC;
    char pad18[0x91 - 0x18];
    unsigned char x91;
    char pad92[2];
    int x94;
    char pad98[0xA0 - 0x98];
    int xA0;
    int count;
    char padA8[0x130 - 0xA8];
    int x130;
} State;

typedef struct Global {
    char pad0[0x1680];
    int x1680;
    void *x1684;
} Global;

typedef struct Arena {
    int x0, x4, x8, xC;
    char pad10[8];
} Arena;

typedef struct NameList {
    char *name[5];
} NameList;

typedef struct Flagged {
    char pad0[4];
    unsigned char x4;
} Flagged;

extern Global D_004D0380, D_004D1A18, D_004D30B0, D_004D4748;
extern char D_002AF2C0[];
extern char D_0029C518[]; /* "NGP" */
extern char D_0029C520[]; /* "RTX" */
extern char D_0029C528[]; /* "TEX" */
extern char D_0029C530[];
extern Flagged *D_0029C468;
extern Flagged *D_0029C46C;
extern Flagged *D_0029DD34;

extern void func_00119E60(void *);
extern void func_0013B328(void);
extern void func_001410B8(State *);
extern void func_001413D0(State *, Global *, void *, NameList *, char *);
extern void func_001418F8(State *, Name *, Name *);
extern char *func_001FA3E8(Name *);
extern char *func_001FA4B0(Name *, int);
extern void func_00206A20(int);
extern void *func_0020D170(Global *, int);
extern void func_0020B228(void *, int, int);
extern void func_00219438(void);
extern void func_00219488(void *, int);
extern int func_002195F0(void *, int, int);
extern Arena *func_00223A98(Arena *, int, int);
extern char *func_00254544(char *, char *);

static inline void ArenaPush(Arena *arena)
{
    int start = (arena->x4 + 3) & ~3;
    int prev = arena->x8;
    arena->x8 = start;
    arena->x4 = start + 4;
    *(int *)start = prev;
}

static inline void ArenaPop(Arena *arena)
{
    arena->x4 = arena->x8;
    arena->x8 = *(int *)arena->x8;
}

void func_001414A0(State *obj, Name *a, Name *b)
{
    char buf[0x40];
    char text[6][16];
    NameList names;
    NameList list;
    Arena arena;
    int handles[5];
    int handle;
    int base;
    int i;

    func_00119E60(D_002AF2C0);
    /* "<= -1" rather than "< 0": same code, but it shifts gcse hash order and so the
       stack slots of the spilled text[] addresses (matches retail only this way) */
    if (obj->x130 <= -1) {
        obj->x130 = D_004D30B0.x1680;
    }
    if (obj->x94 == 0) {
        func_001410B8(obj);
    }
    obj->xA0 = 0;
    obj->x91 = 1;
    func_0013B328();
    for (i = 0; i < 5; i++) {
        func_00206A20(obj->x130 + i);
    }
    obj->count = 3;
    if ((unsigned int)(b->x0 - 35) < 3) {
        obj->count = 2;
    }
    func_00254544(text[0], func_001FA3E8(a));
    func_00254544(text[1], func_001FA4B0(b, 1));
    func_00254544(text[2], func_001FA4B0(b, 2));
    base = obj->x94;
    list.name[0] = text[0];
    list.name[1] = text[1];
    list.name[2] = text[2];
    list.name[3] = text[3];
    list.name[4] = text[4];
    names = list;
    func_00223A98(&arena, base, 0x9B000);
    D_004D0380.x1684 = &arena;
    D_004D30B0.x1684 = &arena;
    D_004D1A18.x1684 = &arena;
    D_004D4748.x1684 = &arena;

    func_001413D0(obj, &D_004D0380, buf, &names, D_0029C518);
    ArenaPush(&arena);
    func_00219438();
    func_001413D0(obj, &D_004D30B0, buf, &names, D_0029C520);
    handle = 0x3F80;
    for (i = 0; i < obj->count; i++) {
        handle = handles[i] = func_002195F0(func_0020D170(&D_004D30B0, obj->x130 + i), obj->x130 + i, handle);
    }
    ArenaPop(&arena);

    func_001413D0(obj, &D_004D1A18, buf, &names, D_0029C528);
    for (i = 0; i < obj->count; i++) {
        func_00219488(func_0020D170(&D_004D1A18, obj->x130 + i), obj->x130 + i);
    }
    ArenaPush(&arena);

    func_001413D0(obj, &D_004D4748, buf, &names, D_0029C530);
    handle = 0x3F80;
    for (i = 0; i < obj->count; i++) {
        func_0020B228(func_0020D170(&D_004D4748, obj->x130 + i), handle, obj->x130 + i);
        handle = handles[i];
    }
    ArenaPop(&arena);

    func_001418F8(obj, a, b);
    obj->x0 = *a;
    obj->xC = *b;
    obj->x91 = 1;
    if (D_0029C468 != 0) {
        D_0029C468->x4 = 1;
    }
    if (D_0029C46C != 0) {
        D_0029C46C->x4 = 1;
    }
    if (D_0029DD34 != 0) {
        D_0029DD34->x4 = 1;
    }
}
