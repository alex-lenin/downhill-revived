/* UNIT 0x00213C08 0x00214550 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: sched order of 4 insns, node+6 pointer reuse, (count-1)*48+hits fused to madd, hit-record store order;
   best diff 20/594 words */
typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct PackedLong { long value; } __attribute__((packed)) PackedLong;

/* Bounding-volume node, quantised to the mesh grid. */
typedef struct Node {
    short min[3];
    short max[3];
    unsigned short leaf : 1;
    unsigned short count : 4;
    unsigned short index : 9;
} Node;

typedef struct Tri {
    unsigned char v[3];
    unsigned char material;
} Tri;

typedef struct Mesh {
    Vec3 *verts;
    unsigned short triCount;
    unsigned short nodeCount;
    float origin[3];
    float scale;
    Node nodes[1];
} Mesh;

typedef struct Contact {
    Vec pos;            /* w = hit time */
    Vec normal;
    float dist;
} Contact;

typedef struct Hit {
    short x0;
    char pad2[4];
    unsigned short material;
    float dist;
    char padC[4];
    Vec pos;
    Vec normal;
} Hit;

typedef struct Box16 { short s[4]; } Box16;
typedef union Scratch {
    short stack[72];    /* node-index stack; shares storage with calc (0x90 bytes) */
    struct { Vec dir; Vec tri[3]; Contact hit; Vec n, v; } calc;
} Scratch;

extern void func_002117F8(void *out, const void *vectors, const void *offset);
extern void func_00211828(void *out, const void *vectors, const void *offset);
extern void func_00211858(void *a, void *b, void *c, const void *scale);
extern void func_00211888(void *matrix, void *a, void *b, void *c, void *outA, void *outB, void *outC);
extern void func_00211930(void *matrix, void *a, void *b, void *c, void *outA, void *outB, void *outC);
extern void func_00221888(void *matrix, void *in, void *out);
extern int func_00213658(Vec *p0, Vec *p1, Vec *dir, float *boxMin, float *boxMax, Vec *tri, Contact *hit, float radius, float best);

static inline float minf(float a, float b) { float r; __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
static inline float maxf(float a, float b) { float r; __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
static inline int clamp16(int v)
{
    int n = v > 0x7FFF ? 0x7FFF : v;
    return n < -0x8000 ? -0x8000 : n;
}

static inline void copy3(Vec *out, Vec3 *in)
{
    float x = in->x, y = in->y, z = in->z;
    out->x = x; out->y = y; out->z = z;
}

static inline void normalize(Vec *out, Vec *in)
{
    float x=in->x,y=in->y,z=in->z;
    float len,inv;
    __asm__ volatile ("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv), "=f"(len) : "f"(x), "f"(y), "f"(z) : "$2");
                    out->x = x * inv;
                    out->y = y * inv;
                    out->z = z * inv;
}

int func_00213C08(Vec *p0, Vec *p1, Vec *matrix, Vec *scale, float *boxMin, float *boxMax,
                  Mesh *mesh, Hit *hits, float radius, int maxHits)
{
    unsigned char list[256];
    Scratch scratch;
    struct { Box16 lo, hi; } bounds;
    unsigned char *end;
    unsigned char *in;
    Tri *tris;
    unsigned char *keep;
    Vec3 *verts;
    int count;
    float best;
    void *out;

    int nodeCount = mesh->nodeCount;

    if (nodeCount != 0) {

        unsigned char *write;
        short *top;
        int pending;
        int idx;
        long packedLo, packedHi;
        Node *nodes = mesh->nodes;
        int x0,y0,z0,x1,y1,z1;
        float *origin = mesh->origin;
        float quantScale = mesh->scale;

        float fx0,fy0,fz0,fx1,fy1,fz1;
        fx0 = boxMin[0] - origin[0];
        fy0 = boxMin[1] - origin[1];
        fz0 = boxMin[2] - origin[2];
        fx1 = boxMax[0] - origin[0];
        fy1 = boxMax[1] - origin[1];
        fz1 = boxMax[2] - origin[2];
        fx0 *= quantScale;
        fy0 *= quantScale;
        fz0 *= quantScale;
        fx1 *= quantScale;
        fy1 *= quantScale;
        fz1 *= quantScale;
        x0 = (int)fx0 - 1;
        y0 = (int)fy0 - 1;
        z0 = (int)fz0 - 1;
        x1 = (int)fx1 + 1;
        y1 = (int)fy1 + 1;
        z1 = (int)fz1 + 1;
        {
            short *lo = bounds.lo.s;
            short *hi = bounds.hi.s;
            lo[0] = clamp16(x0);
            lo[1] = clamp16(y0);
            lo[2] = clamp16(z0);
            lo[3] = -0x8000;
            hi[0] = clamp16(x1);
            hi[1] = clamp16(y1);
            hi[2] = clamp16(z1);
            hi[3] = 0x7FFF;
        }



        write = list;
        packedLo = *(long *)&bounds.lo;
        packedHi = *(long *)&bounds.hi;
        scratch.stack[0] = 0;
        pending = 1;
        top = &scratch.stack[1];
        do {
            pending--;
            idx = *--top;
visit_node:
            {
                Node *node = &nodes[idx];
                int i, n;
                long a, b, r0, r1;
                a = ((PackedLong *)node)->value;
                node = (Node *)((char *)node + 6);
                b = ((PackedLong *)node)->value;
                __asm__ volatile ("pcgth %0,%2,%3\n\tpcgth %1,%4,%5"
                    : "=r"(r0), "=r"(r1) : "r"(a), "r"(packedHi), "r"(packedLo), "r"(b));
                if (!(r0 | r1)) {
                    node = (Node *)((char *)node - 6);
                    if (node->leaf) {
                        n = node->index + node->count;
                        for (i = node->index; i < n; i++)
                            *write++ = i;
                    } else {
                        pending++;
                        idx = node->index;
                        *top++ = idx + 1;
                        goto visit_node;
                    }
                }
            }
        } while (pending != 0);
        end = write;
    } else {
        int i;
        end = list;
        for (i = nodeCount; i < mesh->triCount; i++)
            *end++ = i;
    }

    if (end == list)
        return 0;

    verts = mesh->verts;
    tris = (Tri *)&mesh->nodes[nodeCount];
    keep = list;
    for (in = list; in < end; in++) {
        unsigned char t = *in;
        Tri *tri = &tris[t];
        Vec3 *a = &verts[tri->v[0]];
        Vec3 *b = &verts[tri->v[1]];
        Vec3 *c = &verts[tri->v[2]];

        if (boxMax[0] < minf(minf(a->x, b->x), c->x)) continue;
        if (maxf(maxf(a->x, b->x), c->x) < boxMin[0]) continue;
        if (boxMax[1] < minf(minf(a->y, b->y), c->y)) continue;
        if (maxf(maxf(a->y, b->y), c->y) < boxMin[1]) continue;
        if (boxMax[2] < minf(minf(a->z, b->z), c->z)) continue;
        if (maxf(maxf(a->z, b->z), c->z) < boxMin[2]) continue;
        *keep++ = t;
    }

    count = 0;
    best = 1.0f;
    {
        unsigned char *current;

        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.dir) : "r"(p1), "r"(p0));
        for (current = list; current < keep; current++) {

            Tri *t = &tris[*current];
            Vec3 *a = &verts[t->v[0]];
            Vec3 *b = &verts[t->v[1]];
            Vec3 *c = &verts[t->v[2]];

            if (matrix == 0) {
                copy3(&scratch.calc.tri[0], a);
                copy3(&scratch.calc.tri[1], b);
                copy3(&scratch.calc.tri[2], c);
            } else {
                Vec *tmp = &scratch.calc.hit.pos;
                copy3(&tmp[0], a);
                copy3(&tmp[1], b);
                copy3(&tmp[2], c);
                func_002117F8(tmp, tmp, p0);
                func_00211888(matrix, &tmp[0], &tmp[1], &tmp[2], &scratch.calc.tri[0], &scratch.calc.tri[1], &scratch.calc.tri[2]);
                func_00211858(&scratch.calc.tri[0], &scratch.calc.tri[1], &scratch.calc.tri[2], scale);
                func_00211930(matrix, &scratch.calc.tri[0], &scratch.calc.tri[1], &scratch.calc.tri[2], &tmp[0], &tmp[1], &tmp[2]);
                func_00211828(scratch.calc.tri, tmp, p0);
            }
            scratch.calc.hit.dist = 0.0f;
            if (func_00213658(p0, p1, &scratch.calc.dir, boxMin, boxMax, scratch.calc.tri, &scratch.calc.hit, radius, best) == 0)
                continue;
            if (scratch.calc.hit.pos.w <= 0.0f && best <= 0.0f)
                count++;
            else if (scratch.calc.hit.pos.w < best)
                count = 1;
            else
                continue;
            if (count > maxHits)
                return maxHits;
            {
                int offset = (count - 1) * sizeof(Hit);
                out = (Hit *)((char *)hits + offset);

                ((Hit *)out)->x0 = 1;
                ((Hit *)out)->dist = scratch.calc.hit.dist;
                ((Hit *)out)->material = t->material;
                __asm__ volatile ("lq $2, %1\n\tsq $2, %0" : "=m"(((Hit *)out)->normal) : "m"(scratch.calc.hit.normal) : "$2");
                ((Hit *)out)->pos = scratch.calc.hit.pos;
                if (matrix != 0) {

                    Vec *nrm;
                    float len, inv;

                    func_00221888(matrix, &scratch.calc.hit.normal, &scratch.calc.n);
                    scratch.calc.n.x *= scale->x;
                    scratch.calc.n.y *= scale->y;
                    scratch.calc.n.z *= scale->z;
                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.v) : "r"(&scratch.calc.n), "r"(matrix));
                    nrm = &((Hit *)out)->normal;
                    normalize(nrm, &scratch.calc.v);

                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.v) : "r"(&scratch.calc.hit.pos), "r"(p0));
                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.n) : "r"(&scratch.calc.v), "r"(matrix));
                    scratch.calc.n.x *= 1.0f / scale->x;
                    scratch.calc.n.y *= 1.0f / scale->y;
                    scratch.calc.n.z *= 1.0f / scale->z;
                    func_00221888(matrix, &scratch.calc.n, &((Hit *)out)->pos);
                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(((Hit *)out)->pos) : "r"(&((Hit *)out)->pos), "r"(p0));
                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tmfc1 $2, %2\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.v) : "r"(nrm), "f"(((Hit *)out)->dist) : "$2");
                    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, %0" : "=m"(scratch.calc.n) : "r"(&scratch.calc.v), "r"(matrix));
                    scratch.calc.n.x *= 1.0f / scale->x;
                    scratch.calc.n.y *= 1.0f / scale->y;
                    scratch.calc.n.z *= 1.0f / scale->z;
                    func_00221888(matrix, &scratch.calc.n, &scratch.calc.v);
                    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(len) : "f"(scratch.calc.v.x), "f"(scratch.calc.v.y), "f"(scratch.calc.v.z));
                    ((Hit *)out)->dist = len;
                }
            }
            best = maxf(0.0f, scratch.calc.hit.pos.w);
        }
    }
    return count;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00213C08);
#endif
