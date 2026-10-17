/* UNIT 0x00211BE0 0x00211D68 */
#include "spatial_primitives.h"
#include "spatial.h"
typedef Vector4 Vec;

typedef SpatialFilter Pair;

typedef SpatialHit Hit;

typedef SpatialIterator Iter;








static inline int IsDone(Iter *it)
{
    return it->x30 >= it->x28;
}

#if defined(__mips__)
int func_00211BE0(Vec *pos, Hit *out, float z0, float z1)
#else
int SharedCall00211BE0(struct Vector4 * pos_view, struct Shared00211BE0_Hit * out_view, float z0, float z1)
#endif
{
#if !defined(__mips__)
    Vec * pos = (Vec *)pos_view;
    Hit * out = (Hit *)out_view;
#endif

    float x, y;
    Vec a;
    Vec b;
    Hit hits[16];
    Iter it;
    Pair *p;
    Hit *best;
    int count = 0;

    y = pos->y;
    x = pos->x;
    a.x = x;
    a.y = y;
    a.z = pos->z + z0;
    b.x = x;
    b.y = y;
    b.z = pos->z + z1;
    func_00212048(&it);
    p = func_00210950();
    if (p != 0) {
        it.cell.filter = *p;
    }
    func_00212098(&it, &a, &b, 0.0f);
    while (!IsDone(&it) && count < 16) {
        count += func_00210A18(it.collider, &it.reference, &a, &b, 0.0f, 1.0f, 1.0f, &hits[count], 16 - count);
        func_00212408(&it);
    }
    best = func_00211780(hits, count, 0);
    if (best != 0) {
        *out = *best;
    } else {
        out->x0 = 0;
    }
    return best != 0;
}
