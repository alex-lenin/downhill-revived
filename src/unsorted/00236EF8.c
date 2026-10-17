/* UNIT 0x00236EF8 0x002370E0 */
#include "vector.h"
#include "spatial.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef Matrix4 Matrix;
typedef SpatialFilterBits Bits;
typedef SpatialFilter Pair;
typedef SpatialMatrixNode Node;
typedef SpatialReference Ref;
typedef SpatialIterator Iter;
typedef struct Contact {
    int x0;
    unsigned short x4, x6;
    char pad8[8];
    Matrix matrix;
} Contact;




extern int func_00236E20(int, void *, void *, Ref *, Contact *);

/* 64-byte matrix copy, as libvu0's sceVu0CopyMatrix */
#define COPY_MATRIX(dst, src)     __asm__ volatile ("lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	"                       "sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3"                       : "=m"((dst).row[0]), "=m"((dst).row[1]), "=m"((dst).row[2]), "=m"((dst).row[3])                       : "r"(src) : "$8", "$9", "$10", "$11")
static inline int IsDone(Iter *it) { return it->x30 >= it->x28; }
int func_00236EF8(void *from, void *to, Contact *contacts, int max) {
    Pair pair;
    Iter it;
    int count = 0;
    if (func_00210950() != 0) {
        pair = *func_00210950();
    } else {
        pair.x4.id = 0xFFFF;
        pair.x0 = 0xFFFFFFFFU;
        pair.x4.flags &= 0xFFFEFFFF;
        pair.x4.flags &= 0xFFFDFFFF;
        pair.x4.flags &= 0xFFFBFFFF;
    }
    pair.x4.flags = (pair.x4.flags & 0xFFFBFFFF) | 0x40000;
    func_00212048(&it);
    it.cell.filter = pair;
    func_00212098(&it, (Vector4 *)from, (Vector4 *)to, 0.0f);
    while (!IsDone(&it)) {
        if (func_00236E20(it.collider, from, to, &it.reference, &contacts[count])) {
            contacts[count].x4 = it.x68;
            contacts[count].x6 = it.x6A;
            if (it.reference.node != 0) {
                COPY_MATRIX(contacts[count].matrix, it.reference.node->matrix);
            } else {
                func_00221F20(&contacts[count].matrix);
            }
            count++;
            if (count >= max) break;
        }
        func_00212408(&it);
    }
    return count;
}
