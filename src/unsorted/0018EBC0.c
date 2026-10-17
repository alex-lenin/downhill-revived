/* UNIT 0x0018EBC0 0x0018EC08 */
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
    Link x534;
    char pad[0x550 - 0x534 - sizeof(Link)];
    Link x550;
} Obj;

extern void func_0018E398(Obj *obj);

static inline void Link_Init(Link *link, void *owner)
{
    link->xF = 0;
    link->xE = 0;
    link->x14 = owner;
}

static inline void Link_InitFlag(Link *link, void *owner)
{
    link->x14 = owner;
    link->xF = 0;
    link->xE = 0;
    link->x18 = 1;
}

void func_0018EBC0(Obj *obj)
{
    obj->x530 = 0;
    Link_Init(&obj->x550, obj);
    Link_InitFlag(&obj->x534, obj);
    func_0018E398(obj);
}
