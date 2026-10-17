/* UNIT 0x0022F040 0x0022F568 */
typedef struct View {
    char pad0[4];
    unsigned short width;
    unsigned short height;
    char pad8[8];
    float x10;
    float x14;
} View;
typedef struct Entry { int x0; View *view; char pad8[12]; } Entry;
typedef struct Transform { char pad0[0x30]; float x30, x34, x38, pad3C; float x40, x44, x48; char pad4C[0xA4]; } Transform;
typedef struct Packet { int x0; int x4; int *x8; } Packet;

extern Entry D_0075A9D0[];
extern Transform D_00528360[];
extern unsigned int D_0029DBAC;

typedef union Words { int w[64]; float f[64]; long l[32]; } Words;
#define PI(i) (((Words *)pk->x8)->w[i])
#define PF(i) (((Words *)pk->x8)->f[i])
#define PL(i) (((Words *)pk->x8)->l[i])

void func_0022F040(Packet *pk, long *flags)
{
    int idx = *(int *)0x70000058;
    Entry *entry;
    Transform *t;
    float scale = 16.0f;

    *(int *)0x7000016C = 1;
    t = &D_00528360[idx];
    entry = &D_0075A9D0[idx];

    if (*flags & 0x800000000L) {
        PI(0) = 0x6C0203E5;
        PI(1) = 0;
        PI(2) = 0;
        PF(3) = entry->view->width * 0.0625f - entry->view->x10;
        PF(4) = entry->view->height * 0.0625f - entry->view->x14;
        PI(5) = 0;
        PI(6) = 0;
        PF(7) = 8.0f / entry->view->width;
        PF(8) = 8.0f / entry->view->height;
        PI(9) = 0x640103E7;
        PI(10) = 0;
        PI(11) = 0;
        pk->x8 += 12;
        PI(0) = 0;
        PI(1) = 0;
        PI(2) = 0;
        PI(3) = 0x50000002;
        PL(2) = 0x1000000000008001;
        PL(3) = 0xE;
        PL(4) = 0;
        PL(5) = 8;
        pk->x8 += 12;
        return;
    }
    if (*flags & 0x400000000L) {
        unsigned int odd;

        PI(0) = 0x6C0303E5;
        PF(1) = t->x30 * scale;
        PF(2) = t->x40 * scale;
        PF(3) = entry->view->width * 0.5f - entry->view->x10;
        odd = D_0029DBAC & 1;
        PF(4) = entry->view->height * 0.5f - entry->view->x14 - (float)odd * 0.5f;
        PF(5) = t->x34 * scale;
        PF(6) = t->x44 * scale;
        PF(7) = 1.0f / 1024.0f;
        PF(8) = 1.0f / 256.0f;
        PF(9) = t->x38 * scale;
        PF(10) = t->x48 * scale;
        PI(12) = 0;
        PI(13) = 0;
        PI(14) = 0;
        PI(15) = 0x50000002;
        pk->x8 += 16;
        PL(0) = 0x1000000000008001;
        PL(1) = 0xE;
        PL(2) = 10 | (long)(entry->view->width - 1) << 14 | (long)(entry->view->height - 1) << 34;
        PL(3) = 8;
        pk->x8 += 8;
    } else {
        float k = 0.19999999f;

        PI(0) = 0x6C0303E5;
        PF(1) = t->x30 * scale;
        PF(2) = -t->x40 * scale;
        PF(3) = entry->view->width * 0.5f / k - entry->view->x10;
        PF(4) = -entry->view->x14 - entry->view->height * 0.5f / k;
        PF(5) = t->x34 * scale;
        PF(6) = -t->x44 * scale;
        PF(7) = k / entry->view->width;
        PF(8) = -k / entry->view->height;
        PF(9) = t->x38 * scale;
        PF(10) = -t->x48 * scale;
        PI(12) = 0;
        PI(13) = 0;
        PI(14) = 0;
        PI(15) = 0x50000002;
        pk->x8 += 16;
        PL(0) = 0x1000000000008001;
        PL(1) = 0xE;
        PL(2) = 5;
        PL(3) = 8;
        pk->x8 += 8;
    }
}
