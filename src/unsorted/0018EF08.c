/* UNIT 0x0018EF08 0x0018EF50 */
typedef struct Link {
    char pad0[0xE];
    char xE;
    char xF;
    char pad10[0x14 - 0x10];
    void *x14;
    unsigned char x18;
} Link;

typedef struct Obj {
    char pad0[0x530];
    int x530;
    int x534;
    Link x538;
    char pad[0x554 - 0x538 - sizeof(Link)];
    Link x554;
} Obj;

extern void func_0018E398(Obj *obj);

static inline void Link_Init(Link *link, void *owner)
{
    link->x14 = owner;
    link->xE = 0;
}

static inline void Link_InitFlag(Link *link, void *owner)
{
    link->xE = 0;
    link->x14 = owner;
    link->x18 = 1;
}

void func_0018EF08(Obj *obj)
{
    obj->x530 = 0;
    obj->x534 = 0;
    Link_InitFlag(&obj->x554, obj);
    Link_Init(&obj->x538, obj);
    func_0018E398(obj);
}
