/* UNIT 0x001293D0 0x00129500 */
typedef struct V4 {
    float x, y, z, w;
} V4 __attribute__((aligned(16)));

typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Tag {
    int x0;
    int x4;
    short x8;
    short xA;
} Tag;

typedef struct Slot {
    char pad0[0x4];
    int x4;
    int x8;
    char padC[0x4];
    char x10[0x10];
    int x20, x24, x28, x2C, x30;
    char pad34[0x8];
    int x3C, x40;
    char pad44[0x8];
    int x4C, x50;
    char pad54[0x8];
    int x5C, x60;
    char pad64[0x8];
    Tag x6C;
    char pad78[0x8];
    char x80[0x50];
    V4 xD0[4];
    int x110;
    char pad114[0xC];
    float x120[4];
    float x130[4];
    char pad140[0xC];
    int x14C, x150;
    float x154, x158, x15C, x160, x164;
    int x168, x16C, x170, x174, x178, x17C, x180, x184, x188, x18C, x190;
    int x194;
    int x198, x19C;
} Slot;

extern void func_00254204(void *dst, int value, int size);
extern void func_002608C8(void *p);

static inline void InitTag(Tag *t)
{
    t->x0 = 0;
    t->x4 = 0;
    t->xA = 0;
    t->x8 = 0;
}

static inline void InitMatrix(Slot *slot)
{
    V4 *m = slot->xD0;
    __asm__ volatile("sq $0, %0" : "=m"(*(u128 *)&slot->xD0[0]));
    __asm__ volatile("sq $0, %0" : "=m"(*(u128 *)&slot->xD0[1]));
    __asm__ volatile("sq $0, %0" : "=m"(*(u128 *)&slot->xD0[1]));
    m[3].w = 0.0f;
    m[3].z = 0.0f;
    m[3].y = 0.0f;
    m[3].x = 0.0f;
}

static inline void InitAxis(float *v)
{
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 1.0f;
    v[3] = 0.0f;
}

void func_001293D0(Slot *slot) {
    slot->x4 = 0;
    slot->x8 = 0;
    func_00254204(slot->x10, 0, 0x10);
    slot->x20 = 0;
    slot->x24 = 0;
    slot->x28 = 0;
    slot->x2C = 0;
    slot->x30 = 0;
    slot->x3C = 0;
    slot->x40 = 0;
    slot->x4C = 0;
    slot->x50 = 0;
    slot->x5C = 0;
    slot->x60 = 0;
    InitTag(&slot->x6C);
    func_002608C8(slot->x80);
    InitMatrix(slot);
    slot->x110 = 0;
    InitAxis(slot->x120);
    InitAxis(slot->x130);
    slot->x154 = 1.0f;
    slot->x158 = 1.0f;
    slot->x15C = 1.0f;
    slot->x160 = 1.0f;
    slot->x164 = 1.0f;
    slot->x194 = 300;
    slot->x14C = 0;
    slot->x150 = 0;
    slot->x168 = 0;
    slot->x16C = 0;
    slot->x170 = 0;
    slot->x174 = 0;
    slot->x178 = 0;
    slot->x17C = 0;
    slot->x180 = 0;
    slot->x184 = 0;
    slot->x188 = 0;
    slot->x18C = 0;
    slot->x190 = 0;
    slot->x198 = 0;
    slot->x19C = 0;
}
