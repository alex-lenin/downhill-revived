/* UNIT 0x00206F38 0x00207008 */
typedef struct QTrack {
    char pad0[0x4];
    short count;
    char pad6[0x2];
} QTrack;

typedef struct QHdr {
    float base;
    float scale;
    char pad8[0x8];
} QHdr;

int func_00206F38(QTrack *track, short *out, float time) {
    QHdr *hdr = track ? (QHdr *)track - 1 : (QHdr *)0;
    unsigned char *keys = (unsigned char *)track + 6;
    int mid = (track->count - 1) >> 1;
    struct { int idx; int pad; } ctx;
    int lo, hi;

    /* idx stays a stack local: it is written once and read across the
       search loop, so the compiler keeps its home on the stack */
    ctx.idx = (int)((time - hdr->base) / hdr->scale);
    if (keys[mid * 4 + 4] <= ctx.idx) {
        lo = mid + 1;
        hi = track->count - 1;
        mid = mid + 2;
    } else {
        if (!(ctx.idx < keys[mid * 4])) {
            return mid;
        }
        hi = mid;
        lo = 0;
        mid = 1;
    }
    while (hi - lo >= 2) {
        if (ctx.idx < keys[mid * 4]) {
            hi = mid;
        } else {
            lo = mid;
        }
        mid = (lo + hi) >> 1;
    }
    *out = lo;
    return lo;
}
