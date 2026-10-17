/* UNIT 0x002128F8 0x00212BB0 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vector { u128 q; float f[4]; } Vector;

/* Grid walk state (2D DDA over the cell grid). */
typedef struct Traversal {
    Vector start, end;
    float radius;
    float tNear, tFar;
    float tNextX, tNextY;
    float tDeltaX, tDeltaY;
    float t;
    int cellX, cellY;
    int stepX, stepY;
} Traversal;

typedef struct Grid {
    char pad0[8];
    float cellSize;
    float originX, originY;
    float maxX, maxY;
} Grid;

extern Grid *D_0029DA50;
/* -FLT_MAX / FLT_MAX: z bounds of the grid box. const lets the FLT_MAX load be reused for
   tNextX after the call. */
extern const float D_0029DA60[], D_0029DA64[];
extern float floorf(float), fabsf(float);
extern int func_002145F8(Vector *start, Vector *end, float *tNear, float *tFar,
                         float *lo, float *hi, float radius);
extern void func_00212D90(Traversal *walk);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_002128F8(Traversal *walk, Vector *start, Vector *end, float radius) {
    Vector delta, lo, hi;
    Vector *walkEnd;
    float *pLo, *pHi;
    float loX, loY, hiX, hiY;
    float tNext, tNextY;
    float absDelta, absDeltaY;
    float tDelta, tDeltaY;
    int cell, cellY;
    int step, stepY;

    COPY_VECTOR(walk->start, start);
    walkEnd = &walk->end;
    COPY_VECTOR(*walkEnd, end);
    walk->radius = radius;
    walk->tFar = 1.0f;
    walk->tNear = 0.0f;
    pLo = lo.f;
    pHi = hi.f;
    loX = D_0029DA50->originX;
    loY = D_0029DA50->originY;
    lo.f[0] = loX;
    lo.f[1] = loY;
    pLo[2] = D_0029DA60[0];
    hiX = D_0029DA50->maxX;
    hiY = D_0029DA50->maxY;
    hi.f[1] = hiY;
    hi.f[0] = hiX;
    pHi[2] = D_0029DA64[0];
    if (func_002145F8(&walk->start, walkEnd, &walk->tNear, &walk->tFar, pLo, pHi, radius) == 0) {
        walk->t = walk->tFar;
        return;
    }
    __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %2"
                      : : "r"(walkEnd), "r"(&walk->start), "m"(delta) : "memory");

    walk->tNextX = D_0029DA64[0];
    walk->tDeltaX = 0.0f;
    walk->stepX = 0;
    cell = (int) floorf(((walk->start.f[0] + (walk->tNear * delta.f[0])) - D_0029DA50->originX) / D_0029DA50->cellSize);
    absDelta = fabsf(delta.f[0]);
    walk->cellX = cell;
    if (absDelta > 1e-7f) {
        tDelta = D_0029DA50->cellSize / absDelta;
        walk->tDeltaX = tDelta;
        tNext = ((((float) cell * D_0029DA50->cellSize) + D_0029DA50->originX) - walk->start.f[0]) / delta.f[0];
        walk->tNextX = tNext;
        if (delta.f[0] > 0.0f) {
            walk->tNextX = tNext + tDelta;
        }
        step = -1;
        if (delta.f[0] > 0.0f) {
            step = 1;
        }
        walk->stepX = step;
    }

    walk->tNextY = D_0029DA64[0];
    walk->tDeltaY = 0.0f;
    walk->stepY = 0;
    cellY = (int) floorf(((walk->start.f[1] + (walk->tNear * delta.f[1])) - D_0029DA50->originY) / D_0029DA50->cellSize);
    absDeltaY = fabsf(delta.f[1]);
    walk->cellY = cellY;
    if (absDeltaY > 1e-7f) {
        tDeltaY = D_0029DA50->cellSize / absDeltaY;
        walk->tDeltaY = tDeltaY;
        tNextY = ((((float) cellY * D_0029DA50->cellSize) + D_0029DA50->originY) - walk->start.f[1]) / delta.f[1];
        walk->tNextY = tNextY;
        if (delta.f[1] > 0.0f) {
            walk->tNextY = tNextY + tDeltaY;
        }
        stepY = -1;
        if (delta.f[1] > 0.0f) {
            stepY = 1;
        }
        walk->stepY = stepY;
    }
    walk->t = walk->tNear;
    func_00212D90(walk);
}
