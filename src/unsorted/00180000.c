/* UNIT 0x00180000 0x00180448 .rodata=0x28BF30:0x28BF50 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Ref { int x0, x4, x8, xC; } Ref;
typedef struct Item { unsigned int *type; unsigned char x4; char pad5[0xB]; Vec x10; } Item;
#define ITEM_FLAGS(item) (*(unsigned long *)(item))
typedef struct Frame { char pad[0x10]; Vec x10; } Frame;
typedef struct World { char pad[0x792C]; Frame *x792C; } World;
typedef struct Target { char pad[0x14]; unsigned short x14; short x16; } Target;
typedef struct Track { int x0; Target *x4; short x8; short xA; } Track;
typedef struct Rider {
    int x0;
    int x4;
    Item *x8;
    Item *xC;
    char pad10[0xC4];
    World *xD4;
    Track xD8;
    char padE4[0xBC];
    float x1A0;
    char pad1A4[0xC];
    int x1B0;
    int x1B4;
    char pad1B8[0x58];
    int x210;
} Rider;

extern int func_00238F60(void);
extern void func_0017FDF8(Rider *, Target *);
extern void func_0017FFB8(Rider *, int);
extern void func_00203590(Ref *, int, int, int);
extern void func_002038E0(Ref *, int);
extern void func_00203CB8(Ref *);
extern void func_001808D0(Rider *);
extern void func_00180A10(Rider *);
extern void func_001808F0(Rider *);
extern void func_00180448(Rider *);
extern short func_00163D70(int, Target *);
extern float func_002223F0(Vec *, Target *);
extern void func_002182A8(Item *, int);
extern void func_0022DDD8(int);
extern float func_00235EA8(float, float, float, float);
extern void func_00181618(Rider *, float);

static inline void func_00180000_start(Rider *rider) {
    Ref ref;
    if ((*rider->x8->type & 0xFFFC0000) == 0x14500000) {
        ref.x8 = 0;
        func_00203590(&ref, 0x514, rider->x1B4, 0);
        if (ref.x0) func_002038E0(&ref, 0);
    }
    if ((*rider->x8->type & 0xFFFC0000) == 0x14580000) {
        ref.x8 = 0;
        func_00203590(&ref, 0x516, rider->x1B4, 0);
        if (ref.x0) func_002038E0(&ref, 0);
    }
}

static inline void func_00180000_stop(Rider *rider) {
    Ref ref;
    if ((*rider->x8->type & 0xFFFC0000) == 0x14500000) {
        ref.x8 = 0;
        func_00203590(&ref, 0x514, rider->x1B4, 0);
        if (ref.x0) func_00203CB8(&ref);
    }
    if ((*rider->x8->type & 0xFFFC0000) == 0x14580000) {
        ref.x8 = 0;
        func_00203590(&ref, 0x516, rider->x1B4, 0);
        if (ref.x0) func_00203CB8(&ref);
    }
}

static inline unsigned char func_00180000_reached(Track *track, Vec *pos) {
    short index = track->xA;
    if (index == func_00163D70(track->x0, track->x4) && func_002223F0(pos, track->x4) < 5.0f)
        return 1;
    if (track->x4->x16 == 0 && func_002223F0(pos, track->x4) < 5.0f)
        return 1;
    return 0;
}

static inline float func_00180000_dist(Vec *a, Vec *b) {
    Vec diff;
    float dist;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(diff) : "r"(a), "r"(b) : "memory");
    __asm__ volatile ("mula.s %1,%1\n\tmadd.s %0,%2,%2\n\tsqrt.s %0,%0" : "=&f"(dist) : "f"(diff.x), "f"(diff.y));
    return dist;
}

void func_00180000(Rider *rider) {
    Item *item;
    Track *track;

    item = rider->x8;
    if (item) {
        if (rider->xD4 && func_00180000_dist(&rider->xD4->x792C->x10, &item->x10) < 50.0f) {
            ITEM_FLAGS(item) |= 0x200000000000000UL;
        } else {
            ITEM_FLAGS(rider->x8) &= ~0x200000000000000UL;
        }
    }
    rider->x1B0 += func_00238F60();
    switch (rider->x4) {
    case 0:
        if (!rider->x8) return;
        func_0017FDF8(rider, rider->xD8.x4);
        func_0017FFB8(rider, 1);
        func_00180000_start(rider);
        break;
    case 1:
        func_001808D0(rider);
        break;
    case 2:
        func_00180A10(rider);
        break;
    case 3:
        func_001808F0(rider);
        break;
    case 4:
        break;
    }
    if (!rider->x8) return;
    func_00180448(rider);
    if (!rider->x8) return;
    if (!rider->x8->x4) return;
    if (func_00180000_reached(&rider->xD8, &rider->x8->x10)) {
        func_0017FFB8(rider, 4);
        func_002182A8(rider->x8, 0);
        func_002182A8(rider->xC, 0);
        func_0022DDD8(rider->x210);
        return;
    }
    track = &rider->xD8;
    if (!((track->x4->x14 >> 1) & 1)) return;
    if (func_002223F0(&rider->x8->x10, track->x4) < 10.0f) {
        func_0017FFB8(rider, 4);
        func_0022DDD8(rider->x210);
        func_00180000_stop(rider);
    } else if (func_002223F0(&rider->x8->x10, track->x4) < 50.0f) {
        rider->x1A0 = func_00235EA8(rider->x1A0, 0.073333333f, 0.01f, 0.001f);
        func_00181618(rider, rider->x1A0);
    }
}
