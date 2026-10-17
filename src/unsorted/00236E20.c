/* UNIT 0x00236E20 0x00236EF0 */
#include "spatial.h"
typedef Vector4 Vec4;

typedef struct HitResult {
    char pad0[0x28];
    unsigned long x28;
} HitResult;

typedef struct Collider {
    unsigned int flags;
    int x4;
} Collider;

extern void func_00242970(int, Vec4 *, Vec4 *, HitResult *);

int func_00236E20(Collider *col, void *from, void *to, void *path, Collider **out)
{
    Vec4 start;
    Vec4 end;
    HitResult hit;

    if ((col->flags & 0x3F) != 0xB) {
        return 0;
    }
    func_00212500(path, from, &start, 0.0f);
    func_00212500(path, to, &end, 1.0f);
    func_00242970(col->x4, &start, &end, &hit);
    if ((hit.x28 & 0xFFFFFFFF00000000) == 0x0200020000000000) {
        return 0;
    }
    *out = col;
    return 1;
}
