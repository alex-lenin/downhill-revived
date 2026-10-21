/* UNIT 0x0022E9A0 0x0022F040 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: sched1 issues 1/distance before distance^2 in the frustum setup (f2/f3, v1/a1 swaps there),
   and the shared .sdata/.sbss scalars (defined in 00228BE8.c) are extern here, so lui/%lo instead of $gp
   (+0x10 bytes); best diff 37/423 words with the data defined locally */
typedef unsigned int Qword __attribute__((mode(TI)));
typedef union Vec { Qword q; float f[4]; unsigned int w[4]; } Vec;
typedef struct Mat { Vec r[4]; } Mat;

/* Sony sceVu0CopyMatrix. */
#define COPY_MATRIX(d, s) \
    __asm__ volatile ("lq $8, 0(%4)\n\tlq $9, 16(%4)\n\tlq $10, 32(%4)\n\tlq $11, 48(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((d).r[0]), "=m"((d).r[1]), "=m"((d).r[2]), "=m"((d).r[3]) \
                      : "r"(s) : "$8", "$9", "$10", "$11")

/* Particle emitter, D_0067E180[] (stride 0x2860). */
typedef struct Emitter__e9a0 {
    char pad0[0x2600];
    Vec x2600;                  /* world position, .f[3] = bounding radius */
    char pad2610[0x1B0];
    int x27C0;                  /* flags: 0x800 sorted draw, sign bit = depth relative to view */
    int x27C4;                  /* draw-order depth bias */
    char pad27C8[0x14];
    float x27DC;
    char pad27E0[0x1C];
    float x27FC;
    char pad2800[0x10];
    unsigned int x2810;         /* render mode */
    char pad2814[0xC];
    float x2820;                /* far clip distance */
    char pad2824[8];
    int x282C;                  /* flags: 0x1000000 = extra pass via func_00228090 */
    char pad2830[0x20];
    int x2850;                  /* live particle count */
    char pad2854[4];
    unsigned short x2858;       /* per-view visibility bits */
    char pad285A[6];
} Emitter__e9a0;

typedef struct Link__e9a0 { int next, x4, x8; } Link__e9a0;

extern unsigned char D_0029DBA5;        /* particle rendering enabled */
extern unsigned int D_0029DBAC;         /* frame counter */
extern float *D_0029E2F8, *D_0029E304;  /* GS packet parameter blocks */
extern int D_0029E308;                  /* head of the active emitter list */
extern Vec *D_0067DF80[], *D_00678630[];
extern Mat D_00678680[], D_00678780[], *D_00279AF0[];
extern Vec D_00678880[], D_00678D80[];
extern char D_00679280[][0x1340];
extern Emitter__e9a0 D_0067E180[];
extern Link__e9a0 D_0071F988[];
extern void func_0023D650(int, int *, int *);
extern void func_0023D898(int, float *, float *);
extern Vec *func_0023CF78(int);
extern Mat *func_0023CF58(int);
extern void func_00221888(Mat *, Vec *, Vec *);
extern void func_00228090(Vec *, int, int, float, float);
extern char *func_0023D5F0(int);
extern int func_0021E4E8(void);
extern Vec *func_00224660(Vec *, int, int, int, int, int, int, int, int);
extern Vec *func_0022E560(Vec *, Emitter__e9a0 *, Vec **, Mat *, int);
extern Vec *func_002286F8(Vec *, int, Vec **);
extern int func_002388F0(int, Vec *);
extern void func_0020EC48(Vec *, int);

/* Build the particle DMA chain for one view: frustum-cull every active emitter,
 * depth-sort the visible sorted ones, emit their packets, then roll the view's
 * matrices. Returns the packet end (0 when nothing was emitted). */
Vec *func_0022E9A0(int view, void **out)
{
    int indices[64];
    float depths[64];
    Vec pos, delta, temp;
    int width, height;
    float distance, unused;
    Vec *last;
    int matrixOffset;
    int count, changed;
    Vec **lastPtr;
    Vec *packet, *end;
    int index, i, n;
    float inv, half, horizontal, vertical, horizontalRadius, verticalRadius;
    float r, depth, hs, ws;
    int visibility;
    Emitter__e9a0 *obj;
    Vec *camera;
    char *viewData;
    Vec *state;
    Vec *outputState;

    if (!D_0029DBA5 || view >= 4) return 0;
    last = 0;
    func_0023D650(view, &width, &height);
    count = 0;
    changed = 0;
    func_0023D898(view, &distance, &unused);
    matrixOffset = view << 6;
    lastPtr = &last;
    /* frustum: half-extents at distance, and the slant factors of its planes */
    height *= 2;
    hs = (height * height) * 0.25f + distance * distance;
    ws = (width * width) * 0.25f + distance * distance;
    inv = 1.0f / distance;
    half = inv * 0.5f;
    visibility = 1 << view;
    packet = D_0067DF80[view];
    index = D_0029E308;
    vertical = height * half;
    horizontal = width * half;
    do {
        __asm__ ("sqrt.s %0, %0" : "+f"(hs));
        __asm__ ("sqrt.s %0, %0" : "+f"(ws));
        horizontalRadius = ws * inv;
        verticalRadius = hs * inv;
    } while (0);

    while (index != -1) {
        obj = &D_0067E180[index];
        if (!view) obj->x2858 = 0;
        camera = func_0023CF78(view);
        /* delta.xyz = obj->x2600.xyz - camera.xyz (delta.w = obj->x2600.w) */
        __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                          : "=m"(delta) : "r"(&obj->x2600), "r"(camera));
        func_00221888(func_0023CF58(view), &delta, &pos);
        r = obj->x2600.f[3];
        if (pos.f[1] + r > 1.0f && pos.f[1] - r < obj->x2820 &&
            pos.f[0] + r * horizontalRadius > -pos.f[1] * horizontal &&
            pos.f[0] - r * horizontalRadius < pos.f[1] * horizontal &&
            -pos.f[1] * vertical < pos.f[2] + r * verticalRadius &&
            pos.f[2] - r * verticalRadius < pos.f[1] * vertical) {
            obj->x2858 |= visibility;
            if ((obj->x27C0 & 0x800) && obj->x2850 && (obj->x2810 & 0xFFFFFF8F) != 0x8009) {
                depth = obj->x27C4;
                if (obj->x27C0 < 0) depth -= pos.f[1];
                /* insertion sort by ascending depth */
                i = count;
                n = i + 1;
                for (; i != 0; i--) {
                    if (depth < depths[i - 1]) {
                        depths[i] = depths[i - 1];
                        indices[i] = indices[i - 1];
                    } else break;
                }
                depths[i] = depth;
                indices[i] = index;
                if (obj->x282C & 0x1000000) {
                    temp.q = obj->x2600.q;
                    func_00228090(&temp, obj->x282C, view, obj->x27FC, obj->x27DC);
                }
                count = n;
            }
        }
        if (index != -1) index = D_0071F988[index].next;
    }

    if (count) {
        D_0029E2F8[3] = (float)(D_0029DBAC & 0xFF) * 0.00390625f;
        D_0029E304[3] = (float)(D_0029DBAC & 0x3FF) * 0.0009765625f;
        viewData = func_0023D5F0(view);
        last = packet;
        packet->w[0] = 0x50000000;
        packet->w[1] = (unsigned int)D_00678880;
        packet->w[2] = 0;
        packet->w[3] = 0x11000000;
        packet++;
        last = packet;
        packet->w[0] = 0x50000000;
        packet->w[1] = (unsigned int)D_00678D80;
        packet->w[2] = 0;
        packet->w[3] = 0x11000000;
        state = (Vec *)(viewData + 0x80);
        i = func_0021E4E8();
        packet++;
        last = packet;
        packet->w[0] = 0x10000004;
        packet->w[1] = 0;
        packet->w[2] = 0;
        packet->w[3] = 0x50000004;
        if (i) state = (Vec *)(viewData + 0x1F0);
        func_00224660(packet + 1, 0, 14, 1, 0, 0, 0, 1, 3);
        packet[2].q = state[1].q;
        packet[3].q = state[3].q;
        packet[4].q = state[4].q;
        packet += 5;
    }
    for (i = 0; i < count; i++) {
        changed = 1;
        packet = func_0022E560(packet, &D_0067E180[indices[i]], lastPtr,
                               (Mat *)((char *)D_00678680 + matrixOffset), view);
    }
    end = func_002286F8(packet, view, lastPtr);
    if (end > packet) {
        packet = end;
        changed = 1;
    }
    i = func_002388F0(view, packet);
    if (i) {
        changed = 1;
        packet += i;
    }
    if (changed) {
        packet->w[0] = 0x70000000;
        packet->w[1] = 0;
        packet->w[2] = 0;
        packet->w[3] = 0;
        outputState = D_00678630[view];
        func_0020EC48(&temp, view);
        outputState->q = temp.q;
    }
    COPY_MATRIX(*(Mat *)((char *)D_00678780 + matrixOffset), (Mat *)((char *)D_00678680 + matrixOffset));
    COPY_MATRIX(*(Mat *)((char *)D_00678680 + matrixOffset), D_00279AF0[view]);
    *out = D_00679280[view];
    return changed ? packet : 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0022E9A0);
#endif
