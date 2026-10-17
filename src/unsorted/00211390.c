/* UNIT 0x00211390 0x00211610 .sdata=0x29DA54:0x29DA5C */
#include "spatial_primitives.h"
/* pos must be a struct: field loads are "in struct" MEMs, which gcc 2.95 lets the
 * scheduler move past the D_0029DA54 store (a float* load would alias it). */
typedef struct Vec {
    float x;
    float y;
    float z;
    float w;
} Vec;

typedef struct Obj {
    unsigned int *type; /* type id in bits 18.. of *type */
    char pad4[0xC];
    float pos[4];
    char pad20[0x60];
    int stamp;
} Obj;

typedef struct Node {
    struct Node *next;
    Obj *obj;
} Node;

typedef struct Grid {
    int x0;
    short width;
    short height;
    float cellSize;
    float originX;
    float originY;
    char pad14[8];
    Node **cells;
} Grid;

/* Defined in unit 00210F40; the .extern with its size lets the assembler use $gp as retail does. */
extern Grid *D_0029DA50;
__asm__(".extern D_0029DA50, 4");
int D_0029DA54 = 1;
float D_0029DA58 = -10000.0f;

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

/* Collect up to max objects with type in [minType, maxType] within radius of pos;
 * D_0029DA54 stamps objects so each one is returned once. */
#if defined(__mips__)
int func_00211390(Vec *pos, unsigned int minType, unsigned int maxType, Obj **out, int max, float radius)
#else
int SharedCall00211390(struct Vector4 * pos_view, unsigned int minType, unsigned int maxType, struct Shared00211390_Obj ** out_view, int max, float radius)
#endif
{
#if !defined(__mips__)
    Vec * pos = (Vec *)pos_view;
    Obj ** out = (Obj **)out_view;
#endif

    float inv;
    float px;
    float py;
    float dist;
    short nb[4]; /* cell bounds: [0]=minY [1]=minX [2]=maxY [3]=maxX */
    float delta[4] __attribute__((aligned(16)));
    short yMin;
    short yMax;
    int count;
    int n;
    Node **cell;
    Node *node;
    Obj *obj;
    unsigned int type;
    int stamp;

    px = pos->x;
    inv = 1.0f / D_0029DA50->cellSize;
    D_0029DA54++;
    py = pos->y;
    count = 0;
    nb[1] = (int)fmax(fmin((px - radius - D_0029DA50->originX) * inv, (float)D_0029DA50->width), 0.0f);
    yMin = nb[0] = (int)fmax(fmin((py - radius - D_0029DA50->originY) * inv, (float)D_0029DA50->height), 0.0f);
    nb[3] = (int)fmax(fmin((px + radius - D_0029DA50->originX) * inv, (float)(short)(D_0029DA50->width - 1)), -1.0f);
    yMax = nb[2] = (int)fmax(fmin((py + radius - D_0029DA50->originY) * inv, (float)(short)(D_0029DA50->height - 1)), -1.0f);
    radius *= radius;

    if (yMin <= yMax) do {
        cell = &D_0029DA50->cells[nb[0] * D_0029DA50->width + nb[1]];
        for (n = nb[3] - nb[1]; n >= 0; n--, cell++) {
            for (node = *cell; node != 0; node = node->next) {
                obj = node->obj;
                if (obj == 0 || obj->type == 0)
                    continue;
                stamp = D_0029DA54;
                if (obj->stamp == stamp)
                    continue;
                type = *obj->type >> 18;
                if (type < minType || type > maxType)
                    continue;
                __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                                  : "=m"(delta) : "r"(pos), "r"(obj->pos));
                __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                                  : "=f"(dist) : "f"(delta[0]), "f"(delta[1]), "f"(delta[2]));
                if (dist < radius) {
                    obj->stamp = stamp;
                    count++;
                    *out++ = obj;
                    if (count >= max)
                        return count;
                }
            }
        }
    } while (++nb[0] <= nb[2]);
    return count;
}
