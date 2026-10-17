/* UNIT 0x0021D250 0x0021E3A8 .sdata=0x29DACC:0x29DAE0 */
#include "include_asm.h"

typedef unsigned int u128 __attribute__((mode(TI)));
typedef union { u128 all; unsigned int words[4]; unsigned long dwords[2]; unsigned short shorts[8]; } Packet;
typedef union Vec { u128 q; float f[4]; } Vec;

float D_0029DACC = 255.0f;
float D_0029DAD0 = 0.2f;
int D_0029DAD4 = 1;
float D_0029DAD8 = 1000.0f;
int D_0029DADC = 1;


extern Packet D_0064F980[2][2048];
extern int D_0029E26C;
extern int D_0029E278[2];


typedef struct Shape {
    unsigned int type : 6;
    unsigned int pad0 : 26;
    char pad4[0x18];
    float radius;
} Shape;

typedef struct Owner {
    Shape *shape;
} Owner;

typedef struct Entry {
    float x0, x4, x8;
    char padC[4];
    float x10, x14, x18;
    char pad1C[4];
    Owner *x20;
    int x24;
    int x28;
    float x2C;
    float x30;
    float x34;
    float x38;
    unsigned short x3C;
    char pad3E[2];
} Entry;

extern Entry D_0062F880[];

typedef struct Obj {
    unsigned long flags;
    char pad8[0x34];
    signed char x3C;
} Obj;

typedef struct Ctx {
    char pad0[0x18];
    unsigned int *x18;
} Ctx;

/* bits 48-63 of x0: the low 3 bits are the layer count */
typedef struct Mesh {
    union {
        unsigned long flags;
        struct { unsigned long low : 48, x6 : 16; } bits;
    } x0;
    char pad8[0x24];
    float x2C;
    char pad30[0xC];
    signed char x3C;
} Mesh;

extern Packet D_0062F980[2][4][1024];
extern int D_00660080[2][4];
extern int D_00660190[4];
extern Vec D_00660180;
extern char D_2DC8[];
extern int D_0029E288;

extern void func_00263B48(Entry *, Mesh *, void *, void *, void *, void *);
extern void func_00263B70(void);
extern unsigned int func_00215B58(Obj *obj);
extern void func_00221888(Packet *, Vec *, Packet *);
void func_0021D5D8(Packet *out);
int func_0021D5F8(void);
int func_0021D608(void);

void func_0021D250(Mesh *obj, Ctx *ctx, int buf, int id)
{
    int i, j, count, ok, mask, bits;
    Entry *entry;
    unsigned int tex;
    Packet *p, *head;

    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        bits = *(int *)0x70000208;
        if (!(mask & bits)) {
            continue;
        }
        entry = (Entry *)((i << 6) + (char *)D_0062F880);
        if (id != entry->x24) {
            continue;
        }
        func_00263B48(entry, obj, (char *)obj + 0x10, (char *)obj + 0x20, (char *)obj + 0x30, (char *)obj + 0x40);
        if (obj->x3C >= 0) {
            tex = ctx->x18[obj->x3C] & 0x0FFFFFFF;
        } else {
            tex = func_00215B58((Obj *)obj) & 0x0FFFFFFF;
        }
        count = D_00660080[buf][i];
        if (!func_0021D5F8()) {
            continue;
        }
        ok = 1;
        if (D_0029DADC) {
            func_00263B70();
            ok = func_0021D608();
        }
        if (!ok) {
            continue;
        }
        D_00660190[i]++;
        p = &D_0062F980[buf][i][count];
        count += 9;
        D_0029E288++;
        p->words[1] = tex;
        p->words[3] = 0x6C040000;
        head = p;
        p++;
        func_0021D5D8(p);
        p += 4;
        p->all = 0;
        p->words[3] = 0x6C0103F5;
        p++;
        func_00221888(p - 5, &D_00660180, p);
        p->words[3] = (int)(D_0029DAD0 * 128.0f) | 0x3F80000;
        p++;
        p->all = 0;
        p->words[3] = 0x6C010004;
        p++;
        p->all = 0;
        *(float *)&p->words[2] = obj->x2C;
        p++;
        if (obj->x0.flags & ((unsigned long)0xE000 << 35)) {
            p->all = 0;
            count++;
            p->words[3] = ((obj->x0.bits.x6 & 7) * 3 << 16) | 0x6C000096;
            p++;
            for (j = 0; j < (obj->x0.bits.x6 & 7); j++) {
                p->dwords[0] = 0x598007F80;
                p->dwords[1] = 6;
                p++;
                count += 3;
                p->dwords[0] = 0x60;
                p->dwords[1] = 0x14;
                p++;
                p->all = 0;
                p++;
            }
        }
        head->words[0] = (count - D_00660080[buf][i]) | 0x50000000;
        D_00660080[buf][i] = count + 1;
        p->all = 0;
        p->words[3] = ((unsigned int)D_2DC8 >> 3) | 0x15000000;
        p++;
        p->all = 0;
        p->words[0] = 0x60000000;
    }
}

void func_0021D5D8(Packet *out) {
    __asm__ volatile ("cfc2.i $8, $vi2\n\tsqc2 $vf1, 0(%0)\n\tsqc2 $vf2, 16(%0)\n\tsqc2 $vf3, 32(%0)\n\tsqc2 $vf6, 48(%0)" : : "r"(out) : "$8", "memory");
}

int func_0021D5F8(void)
{
    int vi2;

    __asm__ volatile ("cfc2.i %0, $vi2" : "=r"(vi2));
    return vi2 == 0;
}

int func_0021D608(void) {
    int status, flags;
    __asm__ volatile ("cfc2.i %0, $vi2\n\tcfc2.i %1, $vi5\n\tvmove.xyzw $vf4, $vf29\n\tnop"
                      : "=r"(status), "=r"(flags));
    if (status == 0) return flags != 0 ? 2 : 1;
    return 0;
}

extern int D_0029E260;
extern int D_0029E264;
extern int D_0029E268;
extern int D_0029E26C;
extern int D_0029E270;
extern int D_0029E280;
extern float D_0029E28C;

Entry *func_0021D638(Owner *owner, float x30)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (D_0062F880[i].x3C == 0) {
            D_0062F880[i].x3C = 1;
            D_0062F880[i].x34 = 2500.0f;
            D_0062F880[i].x38 = D_0029DACC;
            D_0062F880[i].x20 = owner;
            D_0062F880[i].x24 = 0;
            D_0062F880[i].x10 = 0;
            D_0062F880[i].x14 = 0;
            D_0062F880[i].x18 = 0;
            if (x30 > 0.0f) {
                D_0062F880[i].x30 = x30;
            } else {
                D_0062F880[i].x30 = 9.0f;
            }
            if (owner->shape != 0 && owner->shape->type == 1) {
                D_0062F880[i].x2C = owner->shape->radius;
            } else {
                D_0062F880[i].x2C = 20.0f;
            }
            return &D_0062F880[i];
        }
    }
    return 0;
}

void func_0021D6F8(Entry *entry, float *pos)
{
    if (entry) {
        entry->x10 = pos[0];
        entry->x14 = pos[1];
        entry->x18 = pos[2];
    }
}

float func_0021D720(int index)
{
    return ((Entry *)((index * 64) + (char *)D_0062F880))->x34;
}

void func_0021D738(Entry *entry, int value)
{
    if (entry != 0) {
        entry->x28 = value;
    }
}

void func_0021D748(Entry *entry, float value)
{
    if (entry != 0) {
        entry->x38 = value;
    }
}

void func_0021D758(Entry *entry, float value)
{
    if (entry) entry->x38 = D_0029DACC * value;
}

void func_0021D770(void)
{
    D_0029DACC = 255.0f;
    D_0029DAD0 = 0.2f;
    D_0029E28C = 6.5f;
    D_0029DAD8 = 1000.0f;
    D_0029E280 = 0;
    D_0029DAD4 = 1;
    D_0029DADC = 1;
    D_0029E26C = 4;
    D_0029E260 = 0;
    D_0029E264 = 0;
    D_0029E268 = 0;
    D_0029E270 = 0;
}

extern int D_0029E284;
typedef struct GsFrame1 {
    unsigned long FBP : 9;
    unsigned long pad09 : 7;
    unsigned long FBW : 6;
    unsigned long pad22 : 2;
    unsigned long PSM : 6;
    unsigned long pad30 : 2;
    unsigned long FBMSK : 32;
} GsFrame1;

/* D_006600A0: draw environment (FRAME_1 register first), D_00660120: clear packet */
extern u128 D_006600A0[8];
extern u128 D_00660120[6];
extern void func_0023C8D8(int a, int b);
extern int func_0023D040(int a);
/* libgraph-style short parameters (sceGsSetDefDrawEnv-like); with int ones the
   scheduler orders the D_0029E280 store differently */
extern int func_002439F8(void *env, short psm, short w, short h, short ztest, short zpsm);
extern int sceGsSetDefClear(void *clear, short ztest, short x, short y, short w, short h,
                            unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned int z);
extern int func_0021E4F0(int i);
extern int func_0021E820(int i);
void func_0021E240(void);

void func_0021D7E8(void)
{
    int i;
    int n;
    Entry *e;

    func_0023C8D8(10, 4);
    D_0029E280 = func_0023D040(4);
    D_0029E284 = 0;
    func_002439F8(D_006600A0, 0, 0x40, 0x40, 0, 0x31);
    ((GsFrame1 *)D_006600A0)->FBP = 0x1FC;
    sceGsSetDefClear(D_00660120, 0, 0x7E0, 0x7E0, 0x40, 0x40, 0, 0, 0, 0, 0);
    e = D_0062F880;
    for (n = 3; n >= 0; n--) {
        e->x20 = 0;
        e->x28 = 0;
        e->x30 = 9.0f;
        e->x3C = 0;
        e->x38 = 80.0f;
        e++;
    }
    for (i = 0; i < 2; i++) {
        D_0029E260 = func_0021E4F0(i);
        D_0029E264 = func_0021E820(i);
    }
    func_0021E240();
}


typedef struct Slot { char pad0[0x20]; int x20; char pad1[0x18]; short x3C; short x3E; } Slot;
int func_0021D910(int value) {
    Slot *entry = (Slot *)D_0062F880;
    int count;
    for (count = 0; count < 4; ++count, ++entry) {
        if (entry->x20 == value) {
            entry->x20 = 0;
            entry->x3C = 0;
            return 1;
        }
    }
    return 0;
}

typedef struct Particle { char pad0[4]; unsigned char active, flags; char pad6[0xA]; float x10, x14, x18; } Particle;
extern int D_0029E284;
extern char D_50[];
extern int func_0023D5F0(int);
extern float *func_0023CF48(void);
extern void func_0021E1B0(float *, float *);
static inline Packet *packet_buffer(Packet *p, int offset) { return (Packet *)(offset + (char *)p); }
static inline void write_scratch(int value) { *(int *)0x70000208 = value; }
static inline int read_scratch(void) { return *(int *)0x70000208; }

void func_0021D950(void *unused, int buffer)
{
    int i, j, result, buffer_offset;
    Entry *entry;
    Particle *particle;
    Packet *packet, *source;
    long state;
    unsigned int type;
    float dx, dy, dz;

    write_scratch(0);
    if (buffer >= 2) return;
    {
        D_0029E288 = 0;
        D_0029E26C = buffer;
        for (i = 0; i < 4; ++i) {
            entry = (Entry *)((char *)D_0062F880 + (i << 6));
            if (D_0029E268 != 0) {
                type = entry->x3C;
                if (type == 1) {
                    particle = (Particle *)entry->x20;
                    if (particle->active != 0) {
                        dx = D_00660180.f[0] * 20.0f;
                        dy = D_00660180.f[1] * 20.0f;
                        dz = D_00660180.f[2] * 20.0f;
                        entry->x0 = dx + particle->x10;
                        entry->x4 = dy + particle->x14;
                        entry->x8 = dz + particle->x18;
                        D_00660190[i] = 0;
                        if ((particle->flags >> buffer) & 1) {
                            write_scratch(read_scratch() | (type << i));
                            packet = &D_0062F980[buffer][i][0];
                        packet->words[0] = 0x10000012;
                        packet->words[3] = 0x5000000D;
                        packet->words[1] = 0;
                        packet->words[2] = ((unsigned int)D_50 >> 3) | 0x15000000;
                        result = func_0023D5F0(buffer);
                        ++packet;
                        source = (Packet *)(D_0029E284 == 0 ? result + 0x80 : result + 0x1F0);
                        j = 0;
                        do {
                            packet->all = source->all;
                            if (j == 0) packet->shorts[0] = 0x800C;
                            ++j;
                            ++packet;
                            ++source;
                        } while (j < 9);
                        packet->dwords[0] = 0;
                        packet->dwords[1] = 0x3F;
                        ++packet;
                        if (D_0029E270 == 0) {
                            packet->dwords[0] = ((long)0x8000 << 23) | 0x46;
                        } else {
                            packet->dwords[0] = ((long)0x8000 << 23) | 1;
                        }
                        packet->dwords[1] = 0x42;
                        ++packet;
                        packet->dwords[0] = (long)0x8080 << 17;
                        packet->dwords[1] = 0x4E;
                        ++packet;
                        packet->dwords[0] = 5;
                        packet->dwords[1] = 8;
                        ++packet;
                        packet->all = 0;
                        packet->words[3] = 0x6C0403F0;
                        ++packet;
                            func_0021E1B0(func_0023CF48(), (float *)packet);
                            D_00660080[buffer][i] = 0x13;
                        }
                    }
                }
            }
        }
    }
}

void func_0021DC08(void) {
    int buf = D_0029E26C;

    if (buf < 2) {
        D_0029E278[buf] = 0;
        D_0064F980[buf][0].all = 0x70000000;
    }
}

extern u128 D_006600A0[8];
extern u128 D_00660120[6];
extern float *func_0023CF30(int);
extern float *func_0023CF78(int);

typedef struct { volatile unsigned long lo; unsigned long hi; } Tag;

void func_0021DC50(float *pos, int a1, int buf, int entry)
{
    u128 *p;
    Tag *tag;
    float *q;
    float scale;
    int i;
    u128 *src1;
    u128 *src2;
    int bshift;

    src1 = D_006600A0;
    src2 = D_00660120;
    p = &D_0064F980[buf][D_0029E278[buf]].all;
    ((int *)p)[0] = 0x10000016;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = ((unsigned int)D_50 >> 3) | 0x15000000;
    ((int *)p)[3] = 0x5000000F;
    p++;
    tag = (Tag *)p;
    tag->lo = 0x100000000000800EL;
    tag->hi = 0xE;
    p++;
    for (i = 7; i >= 0; i--) {
        *p++ = *src1++;
    }
    bshift = buf << 2;
    for (i = 5; i >= 0; i--) {
        *p++ = *src2++;
    }
    *p = 0;
    ((int *)p)[3] = 0x6C0603F0;
    p++;
    func_0021E1B0(func_0023CF30(a1), (float *)p);
    p += 4;
    q = func_0023CF78(buf);
    ((float *)p)[0] = q[0] + pos[0];
    ((float *)p)[1] = q[1] + pos[1];
    ((float *)p)[2] = q[2] + pos[2];
    ((float *)p)[3] = 1.0f;
    p++;
    scale = ((Entry *)((entry << 6) + (char *)D_0062F880))->x38;
    ((float *)p)[0] = scale;
    ((float *)p)[1] = scale;
    ((float *)p)[2] = scale;
    ((float *)p)[3] = scale;
    *(int *)(bshift + (char *)D_0029E278) += 0x17;
}

extern char D_30A0[];

extern unsigned int func_00215B58(Obj *obj);
extern void func_0021D5D8(Packet *out);

void func_0021DE28(Obj *obj, Ctx *ctx) {
    Packet *packet;

    if (!(obj->flags & ((unsigned long)0x8000 << 40))) {
        packet = &D_0064F980[D_0029E26C][D_0029E278[D_0029E26C]];
        packet->words[0] = 0x50000005;
        if (obj->x3C >= 0) {
            packet->words[1] = ctx->x18[obj->x3C];
        } else {
            packet->words[1] = func_00215B58(obj);
        }
        packet->words[2] = 0x10000000;
        packet->words[3] = 0x6C040000;
        packet++;
        func_0021D5D8(packet);
        packet += 4;
        packet->all = 0;
        packet->words[3] = ((unsigned int)D_30A0 >> 3) | 0x15000000;
        D_0029E278[D_0029E26C] += 6;
    }
}

extern char D_0065F980[2][480];
extern char D_0065FD80[2][384];

void func_0021DF38(int index)
{
    Packet *packet = &D_0064F980[D_0029E26C][D_0029E278[D_0029E26C]];

    if (D_00660190[index] > 0) {
        int count = D_0029DAD4;

        if (count > 0) {
            do {
                unsigned int address = (unsigned int)D_0065F980[D_0029E26C];
                unsigned int tag = D_0029E260 | 0x30000000;

                --count;
                packet->all = 0;
                packet->words[0] = tag;
                packet->words[1] = address;
                packet++;
            } while (count);
        }
        {
            unsigned int tag = D_0029E264 | 0x30000000;
            int buf = D_0029E26C;
            unsigned int address = (unsigned int)D_0065FD80[buf];
            int *out;
            int total;

            packet->all = 0;
            out = &D_0029E278[buf];
            total = *out + D_0029DAD4;
            packet->words[1] = address;
            packet->words[0] = tag;
            packet++;
            packet->all = 0x50000000;
            packet->words[1] = (unsigned int)D_0062F980[buf][index];
            *out = total + 2;
        }
    }
}

void func_0021E068(void) {
    u128 *packet = &D_0064F980[D_0029E26C][D_0029E278[D_0029E26C]].all;

    *packet = 0x70000000;
}

INCLUDE_ASM("asm/nonmatchings/text", func_0021E0B0);

typedef struct { char pad[0x10]; Packet *x10; } Holder;
extern int D_0029E288;

u128 *func_0021E0B8(Holder *obj) {
    u128 *packet = &D_0064F980[D_0029E26C][D_0029E278[D_0029E26C]].all;

    if (D_0029E288 > 0) {
        obj->x10->all = 0;
        obj->x10->words[0] = 0x20000000;
        obj->x10->words[1] = (unsigned int)D_0064F980[D_0029E26C];
        return packet;
    }
    return 0;
}

typedef struct { char pad[0x8]; Packet *x8; } Holder8;

u128 *func_0021E130(Holder8 *obj) {
    u128 *packet = &D_0064F980[D_0029E26C][D_0029E278[D_0029E26C]].all;

    if (D_0029E288 > 0) {
        obj->x8->all = 0;
        obj->x8->words[0] = 0x20000000;
        obj->x8->words[1] = (unsigned int)D_0064F980[D_0029E26C];
        obj->x8++;
        return packet;
    }
    return 0;
}

void func_0021E1B0(float *src, float *dst) {
    dst[0] = src[0];
    dst[1] = src[4];
    dst[2] = src[8];
    dst[3] = src[12];
    dst += 4;
    dst[0] = src[1];
    dst[1] = src[5];
    dst[2] = src[9];
    dst[3] = src[13];
    dst += 4;
    dst[0] = src[2];
    dst[1] = src[6];
    dst[2] = src[10];
    dst[3] = src[14];
    dst += 4;
    dst[0] = src[3];
    dst[1] = src[7];
    dst[2] = src[11];
    dst[3] = src[15];
}

extern void func_00230258(Vec *, int);
extern void func_002226C8(void *, Vec *);
extern void func_0023D8D8(int, float, float);
extern void func_0023CE00(int);

void func_0021E240(void) {
    Vec v;
    Vec *dir;

    dir = &D_00660180;
    func_00230258(&v, 0);
    dir->q = v.q;
    if (dir->f[2] >= 0.0f) {
        if (dir->f[2] < 0.92f) {
            float x = D_00660180.f[0];
            float y = dir->f[1];
            float len;
            __asm__ volatile ("mula.s %1, %1\n\tmadd.s %0, %2, %2\n\tsqrt.s %0, %0" : "=f"(len) : "f"(x), "f"(y));
            len *= 0.92f;
            dir->f[2] = 0.92f;
            D_00660180.f[0] = x * len;
            dir->f[1] = y * len;
        }
    } else if (dir->f[2] > -0.92f) {
        float x = D_00660180.f[0];
        float y = dir->f[1];
        float len;
        __asm__ volatile ("mula.s %1, %1\n\tmadd.s %0, %2, %2\n\tsqrt.s %0, %0" : "=f"(len) : "f"(x), "f"(y));
        len *= -0.92f;
        dir->f[2] = 0.92f;
        D_00660180.f[0] = x * len;
        dir->f[1] = y * len;
    }
    D_00660180.f[3] = 1.0f;
    if (D_0029E280) {
        v.f[0] = -D_00660180.f[0];
        v.f[1] = -D_00660180.f[1];
        v.f[2] = -D_00660180.f[2];
        v.f[3] = 1.0f;
        func_002226C8((char *)D_0029E280 + 0x40, &v);
        func_0023D8D8(4, D_0029DAD8, 0.98f);
        func_0023CE00(4);
    }
}
