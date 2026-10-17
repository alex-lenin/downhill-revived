/* UNIT 0x00214A18 0x00214BA0 */
#include "spatial.h"
typedef Vector4 Vec;

typedef SpatialFilter Pair;

typedef SpatialHit Hit;

typedef SpatialIterator Iter;








static inline int IsDone(Iter *it)
{
    return it->x30 >= it->x28;
}

static inline void SetHits(Hit *hits, int start, int n, unsigned short x, unsigned short y) {
    Hit *h;
    int i;
    for (i=0; i<n; i++) { h = hits + start + i; h->x2=x; h->x4=y; }
}
int func_00214A18(Vec *a, Vec *b, Hit *hits, int max, float f0, float f1, float f2)
{
    Iter it;
    Pair *p;
    int count = 0;
    int n, i;
    Hit *h;
    func_00212048(&it);
    p = func_00210950();
    if (p) it.cell.filter = *p;
    func_00212098(&it, a, b, f0);
    if (!IsDone(&it)) do {
        n = func_00210A18(it.collider, &it.reference, a, b, f0, f1, f2, hits + count, max-count);
        if (n) {
            SetHits(hits, count, n, it.x68, it.x6A);
        }
        count += n;
        if (count >= max) break;
        func_00212408(&it);
    } while (!IsDone(&it));
    if (!count) hits->x0 = 0;
    return count;
}
