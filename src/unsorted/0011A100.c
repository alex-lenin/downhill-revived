/* UNIT 0x0011A100 0x0011A438 */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));

/* gcc 2.95 vtable entry: this-delta, then function */
typedef struct VtblEntry { short delta; short pad; void (*fn)(); } VtblEntry;
typedef struct Module { VtblEntry *vtbl; } Module;

typedef struct Rider {
    char pad0[0x4098];
    unsigned long flags; /* 0x4098, bit 2 = rigid module active */
    char pad40A0[0x20];
    char ctrl[0x6420 - 0x40C0]; /* 0x40C0 */
    Module rigid; /* 0x6420 */
    char pad6424[0x792C - 0x6424];
    char *x792C; /* position vector at +0x10 */
    char pad7930[0x7A5C - 0x7930];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;

typedef struct Race {
    Rider riders[10];
    char pad4D620[0x501A8 - 0x4D620];
    int numRiders; /* 0x501A8 */
    char pad501AC[8];
    int x501B4;
    char pad501B8[8];
    int x501C0;
} Race;

extern char D_0043EEC0[];
extern char D_0043F9F0[];
extern void func_0019EE98(void *);
extern int func_001A8DE8(void *);
extern void func_0011A050(Race *);
extern void func_0012C8B0(Rider *);
extern void func_0012D248(Rider *);
extern void func_0012D360(Rider *);
extern void func_0012D380(Rider *);
extern void func_0012D3C8(Rider *);
extern void func_0011A438(Race *);
extern void func_00134688(void *, void *);
extern void func_0011A548(Race *, int);
extern void func_0011A938(Race *);

#define SUB(d, a, b) __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*(d)) : "r"(a), "r"(b))
#define LENGTH(len, v) __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"((v).x), "f"((v).y), "f"((v).z))

/* bit 2 of flags: the rider's rigid module is active */
static inline int HasRigid(Rider *r) {
    return (unsigned char)(r->flags >> 2) & 1;
}

/* position vector */
static inline char *Rider_Pos(Rider *r) {
    return r->x792C + 16;
}

void func_0011A100(Race *race) {
    int count = race->numRiders;
    int i, j, k;
    Module *rigid;
    Vec diff;
    float length;

    func_0019EE98(D_0043EEC0);
    if (!func_001A8DE8(D_0043F9F0)) {
        return;
    }
    if (++race->x501B4 >= race->numRiders) {
        race->x501B4 = 0;
    }
    if (--race->x501C0 < 0) {
        func_0011A050(race);
    }
    for (i = 0; i < count; i++) {
        func_0012C8B0(&race->riders[i]);
    }
    for (i = 0; i < count; i++) {
        func_0012D248(&race->riders[i]);
    }
    for (i = 0; i < count; i++) {
        func_0012D360(&race->riders[i]);
    }
    for (i = 0; i < count; i++) {
        func_0012D380(&race->riders[i]);
    }
    for (i = 0; i < count; i++) {
        func_0012D3C8(&race->riders[i]);
    }
    func_0011A438(race);
    for (i = 0; i < count; i++) {
        if (race->riders[i].x7A5C) {
            for (j = i + 1; j < count; j++) {
                if (race->riders[j].x7A5C) {
                    func_00134688(race->riders[i].ctrl, race->riders[j].ctrl);
                }
            }
        }
    }
    for (i = 0; i < count; i++) {
        if (HasRigid(&race->riders[i])) {
            continue;
        }
        for (k = 0; k < count; k++) {
            if (HasRigid(&race->riders[k])) {
                SUB(&diff, Rider_Pos(&race->riders[i]), Rider_Pos(&race->riders[k]));
                LENGTH(length, diff);
                if (length < 5.0f) {
                    rigid = &race->riders[k].rigid;
                    rigid->vtbl[1].fn((char *)rigid + rigid->vtbl[1].delta, &race->riders[i], 0);
                }
            }
        }
    }
    func_0011A548(race, count);
    func_0011A938(race);
}
