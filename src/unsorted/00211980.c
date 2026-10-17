/* UNIT 0x00211980 0x00211BE0 */
#include "ground_probe.h"
#include "spatial.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef GroundProbeVector Vec;
typedef SpatialFilterBits Bits;
typedef SpatialFilter Pair;
typedef SpatialHit Hit;
typedef SpatialIterator Iter;





extern float D_0029DA5C[];
#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
static inline int IsDone(Iter *it) { return it->x30 >= it->x28; }
float func_00211980(Vec *pos, Vec *out, int flag, float height) {
    Vec a;
    Vec b;
    Hit hits[16];
    Pair pair;
    Iter it;
    Hit *best;
    Hit *hit;
    int count = 0;
    int i;
    float bz, az;
    float result;
    a.q = pos->q;
    b.q = a.q;
    bz = b.f[2] - 4096.0f;
    az = a.f[2] + height;
    b.f[2] = bz;
    a.f[2] = az;
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
    func_00212048(&it);
    it.cell.filter = pair;
    func_00212098(&it, (Vector4 *)&a, (Vector4 *)&b, 0.0f);
    while (!IsDone(&it) && count < 16) {
        count += func_00210A18(it.collider, &it.reference, &a, &b, 0.0f, 1.0f, 1.0f, &hits[count], 16 - count);
        func_00212408(&it);
    }
    if (count != 0) {
        best = hits;
        for (i = 1; i < count; i++) {
            hit = &hits[i];
            if (hit->x10.w < best->x10.w) best = hit;
        }
        COPY_VECTOR(*out, &best->x20);
        return pos->f[2] - best->x10.z;
    }
    out->f[3] = 0;
    out->f[2] = 1.0f;
    result = D_0029DA5C[0];
    out->f[1] = 0;
    out->f[0] = 0;
    return result;
}
