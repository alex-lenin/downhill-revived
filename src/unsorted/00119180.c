/* UNIT 0x00119180 0x001195A8 */
#include "track_bins.h"
#include "track_projection.h"
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Node {
    Vec pos;
    char pad10[4];
    unsigned short flags;
    char pad16[2];
    unsigned short next;
} Node;

struct Track {
    char pad0[0x1004C];
    int count;
};


extern Node *func_001177F8(struct Track *track, int index);
extern int func_001195A8(struct Track *track, Node *node);
extern int func_00119648(struct Track *track, int index);
extern void func_00119700(struct Track *track, short *index, short *next);
extern void func_002225A8(void *, Vec *);

static inline void VecCopy(Vec *dst, Vec *src)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vsub.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "vadd.xyz $vf11, $vf11, $vf12\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecScale(Vec *out, Vec *v, float scale)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "mfc1 $2, %2\n"
        "qmtc2.ni $2, $vf12\n"
        "vmulx.xyz $vf11, $vf11, $vf12x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(v), "f"(scale) : "$2");
}

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float scale)
{
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n"
        "lqc2 $vf12, 0x0(%2)\n"
        "mfc1 $2, %3\n"
        "qmtc2.ni $2, $vf13\n"
        "vaddax.xyz ACC, $vf11, $vf0x\n"
        "vmaddx.xyz $vf11, $vf12, $vf13x\n"
        "sqc2 $vf11, %0\n"
        : "=m"(*out) : "r"(a), "r"(b), "f"(scale) : "$2");
}

static inline float VecDot(Vec *a, Vec *b)
{
    float r;
    __asm__ volatile (
        "mula.s %1, %2\n"
        "madda.s %3, %4\n"
        "madd.s %0, %5, %6\n"
        : "=f"(r) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline void Normalize(Vec *v)
{
    float length, inv, x = v->x, y = v->y, z = v->z;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3" : "=f"(length) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0, %1, %2" : "=&f"(inv) : "f"(1.0f), "f"(length));
    v->x = x * inv;
    v->y = y * inv;
    v->z = z * inv;
}

/* Projects pos onto the track segment it lies on; writes the point to out and
   the segment direction to dir. Returns 1 when it moved to the next segment. */
#if defined(__mips__)
int func_00119180(struct Track *track, Vec *pos, Vec *out, void *dirOut, int flag)
#else
int func_00119180(struct TrackProjectionState *track_view, struct Vector4 *pos_view, struct Vector4 *out_view, float *angles, int flag)
#endif
{
#if !defined(__mips__)
    struct Track *track=(struct Track *)track_view;
    Vec *pos=(Vec *)pos_view, *out=(Vec *)out_view;
    void *dirOut=angles;
#endif
    short idx[2];
    Vec toPos;
    Vec dir;
    float length;
    float inv;
    float t;
    float x, y, z;
    int result;

    func_00117A78((PathBins *)((char *)track + 0x10070),(float *)(pos),(short *)(&idx[0]),(short *)(&idx[1]));
    result = 0;
    if (idx[0] < 0 || idx[1] < 0) {
        goto ret;
    }
    VecSub(&toPos, pos, &func_001177F8(track, idx[0])->pos);
    VecSub(&dir, &func_001177F8(track, idx[1])->pos, &func_001177F8(track, idx[0])->pos);
    x = dir.x;
    y = dir.y;
    z = dir.z;
    /* libvu0-style normalize: length and 1/length */
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    dir.x = x * inv;
    dir.y = y * inv;
    dir.z = z * inv;

    if ((func_001177F8(track, idx[0])->flags >> 3) & 1) {
        result = 1;
        VecCopy(out, &func_001177F8(track, idx[1])->pos);
        idx[0] = idx[1];
        idx[1] = func_001177F8(track, idx[0])->next;
        VecSub(&dir, &func_001177F8(track, idx[1])->pos, &func_001177F8(track, idx[0])->pos);
        Normalize(&dir);
        goto done;
    }
    if (flag == 0 && func_001195A8(track, func_001177F8(track, idx[1]))) {
        result = 1;
        if ((func_001177F8(track, idx[1])->flags >> 2) & 1) {
            func_00119700(track, &idx[0], &idx[1]);
        } else if (!((func_001177F8(track, idx[0])->flags >> 9) & 1)) {
            idx[0] = func_00119648(track, idx[0]);
        }
        VecCopy(out, &func_001177F8(track, idx[0])->pos);
        idx[1] = func_001177F8(track, idx[0])->next;
        VecSub(&dir, &func_001177F8(track, idx[1])->pos, &func_001177F8(track, idx[0])->pos);
        Normalize(&dir);
        goto done;
    }
    t = VecDot(&toPos, &dir);
    if (t > 0.0f) {
        if (t < length) {
            VecScale(out, &dir, t);
            VecAdd(out, out, &func_001177F8(track, idx[0])->pos);
            goto done;
        }
        if (idx[1] == track->count - 1) {
            VecAddScaled(out, &func_001177F8(track, idx[1])->pos, &toPos, 0.5f);
            goto done;
        }
        VecCopy(out, &func_001177F8(track, idx[1])->pos);
        goto done;
    }
    if (idx[0] == track->count - 1) {
        VecAddScaled(out, &func_001177F8(track, idx[0])->pos, &toPos, 0.5f);
        goto done;
    }
    VecCopy(out, &func_001177F8(track, idx[0])->pos);
done:
    func_002225A8(dirOut, &dir);
ret:
    return result;
}
