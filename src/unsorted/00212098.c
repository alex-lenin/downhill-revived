/* UNIT 0x00212098 0x002121E0 */
#include "spatial.h"
typedef Vector4 Vec;

typedef struct Grid {
    int x0;
    short width;
    short height;
    float cellSize;
    float originX;
    float originY;
} Grid;

typedef SpatialIterator Iter;

extern Grid *D_0029DA50;


static inline float fmin(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float fmax(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline int imax(int a, int b)
{
    return a > b ? a : b;
}

static inline int imin(int a, int b)
{
    return a < b ? a : b;
}

/* Set up an iterator over the grid cells covered by the box a..b grown by margin. */
void func_00212098(Iter *it, Vec *a, Vec *b, float margin)
{
    Grid *grid;
    float inv;

    it->min.x = fmin(a->x, b->x) - margin;
    it->min.y = fmin(a->y, b->y) - margin;
    it->min.z = fmin(a->z, b->z) - margin;
    it->max.x = fmax(a->x, b->x) + margin;
    it->max.y = fmax(a->y, b->y) + margin;
    it->max.z = fmax(a->z, b->z) + margin;
    grid = D_0029DA50;
    inv = 1.0f / grid->cellSize;
    it->x24 = imax((it->min.x - grid->originX) * inv, 0);
    it->x20 = imax((it->min.y - grid->originY) * inv, 0);
    it->x2C = imin((int)((it->max.x - grid->originX) * inv) + 1, grid->width);
    it->x28 = imin((int)((it->max.y - grid->originY) * inv) + 1, grid->height);
    it->x30 = it->x20;
    it->x34 = it->x24;
    func_00212408(it);
}
