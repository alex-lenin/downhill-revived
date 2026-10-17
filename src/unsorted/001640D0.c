/* UNIT 0x001640D0 0x00164170 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Vertex {
    Vec pos;
    char pad10[0x10];
} Vertex;

typedef struct Mesh {
    Vertex *verts;
} Mesh;

typedef struct Node {
    char pad0[0x16];
    short count;
    short indices[1];
} Node;

extern float D_0029C7E0[];

static inline void VecSub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline void VecZero(Vec *out)
{
    __asm__ volatile ("sqc2 $vf0,%0" : "=m"(*out));
}

static inline float VecLengthSq(Vec *v)
{
    float r;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3"
                      : "=f"(r) : "f"(v->x), "f"(v->y), "f"(v->z));
    return r;
}

/* returns the vertex of node nearest to ref (or to the origin if ref is null) */
Vertex *func_001640D0(Mesh *mesh, Node *node, Vec *ref)
{
    Vec d;
    Vertex *best;
    Vertex *v;
    float bestDist;
    float dist;
    int i;

    best = (Vertex *)node; /* retail returns node itself if no vertex beats the initial distance */
    bestDist = D_0029C7E0[0];
    for (i = 0; i < node->count; i++) {
        v = &mesh->verts[node->indices[i]];
        if (ref != 0) {
            VecSub(&d, &v->pos, ref);
        } else {
            VecZero(&d);
        }
        dist = VecLengthSq(&d);
        if (dist < bestDist) {
            bestDist = dist;
            best = v;
        }
    }
    return best;
}
