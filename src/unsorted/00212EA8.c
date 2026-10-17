/* UNIT 0x00212EA8 0x00213098 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec { u128 q; float f[4]; } Vec;
typedef struct Bits {
    unsigned id:16;
    unsigned flag1:1;
    unsigned flag2:1;
    unsigned flag3:1;
    unsigned reserved:13;
} Bits;
typedef struct Pair {
    unsigned int x0;
    union { unsigned short id; unsigned int flags; Bits bits; } x4;
} Pair;
typedef struct Hit {
    unsigned short x0;
    unsigned short x2, x4;
    char pad6[0x16];
    float x1C;
    char pad20[0x10];
} Hit __attribute__((aligned(16)));
typedef struct Iter {
    char pad0[0x28];
    float end;
    float timeA;
    float timeB;
    float stepA;
    float stepB;
    float now;
    int posA;
    int posB;
    int strideA;
    int strideB;
    char pad50[0x18];
    Pair x68;
    Pair x70;
    int x78;
    char pad7C[4];
    unsigned short x80, x82;
    char pad84[0xC];
} Iter;
extern Pair *func_00210950(void);
extern Iter *func_002128B8(Iter *);
extern void func_002128F8(Iter *, Vec *, Vec *, float);
extern int func_00210A18(int, Pair *, Vec *, Vec *, float, float, float, Hit *, int);
extern void func_00212D90(Iter *);
extern int func_00212E80(Iter *);
extern float D_0029DA68[];

int func_00212EA8(Vec *a, Vec *b, Hit *out, int flag)
{
    float best;
    Hit hits[16];
    Pair pair;
    Iter it;
    int n;
    int done;
    int rem;
    Hit *h;
    unsigned int t;

    best = D_0029DA68[0];
    if (out != 0) {
        out->x0 = 0;
    }
    if (func_00210950() != 0) {
        pair = *func_00210950();
    } else {
        pair.x4.id = 0xFFFF;
        pair.x0 = 0xFFFFFFFFU;
        pair.x4.flags &= 0xFFFEFFFF;
        pair.x4.flags &= 0xFFFDFFFF;
        pair.x4.flags &= 0xFFFBFFFF;
    }
    pair.x4.flags = (pair.x4.flags & 0xFFFDFFFF) | ((flag & 1) << 17);
    func_002128B8(&it);
    it.x68 = pair;
    func_002128F8(&it, a, b, 0.0f);
    while ((done = func_00212E80(&it)) == 0) {
        n = func_00210A18(it.x78, &it.x70, a, b, 0.0f, 1.0f, 1.0f, hits, 16);
        if (n != 0 && out == 0) {
            return 0;
        }
        if (done < n) {
            h = hits;
            rem = n;
            do {
                if (h->x1C < best) {
                    *out = *h;
                    best = out->x1C;
                    out->x2 = it.x80;
                    out->x4 = it.x82;
                    it.end = best;
                }
                h++;
            } while (--rem != 0);
        }
        func_00212D90(&it);
    }
    if (out == 0) {
        return 1;
    }
    return out->x0 < 1;
}
