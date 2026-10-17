/* UNIT 0x0012E780 0x0012E940 */
#include "object_registry.h"
struct Vec { float x, y, z, w; } __attribute__((aligned(16)));
struct Body { char pad0[0x10]; Vec pos; char pad20[0x20]; Vec x40; };
struct Rider { char pad0[0x792C]; Body *body; };
struct Node { unsigned int *flags; char pad4[0xC]; Vec pos; };
class Target {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Body *v3();
};
extern "C" int func_00211390(Vec *, int, int, Node **, int, float);

extern "C" void func_00221888(Vec *, Vec *, Vec *);

static inline void sub(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

static inline float norm(Vec *out, Vec *v)
{
    float len, inv, x = v->x, y = v->y, z = v->z;
    /* SDK length and reciprocal sequence, as in the promoted vector helpers. */
    __asm__ volatile ("mula.s %2, %2\n\tmadda.s %3, %3\n\tmadd.s %1, %4, %4\n\tsqrt.s %1, %1\n\t"
                      "lui $2, 0x3f80\n\tmtc1 $2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len) : "f"(x), "f"(y), "f"(z), "f"(z) : "$2");
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return len;
}

/* Finds the nearest target around the rider, stores it in *found and the
   normalized direction to it in out. Returns 1 if a target was found. */
int func_0012E780(Rider *self, Vec *out, Target **found)
{
    Node *nodes[10];
    Vec diff;
    Target *best = 0;
    int success = 0;
    float distance = 99999.0f;
    int i, total;
    Target *candidate;
    float length;

    total = func_00211390(&self->body->pos, 1200, 4499, nodes, 10, 20.0f);
    for (i = 0; i < total; i++) {
        candidate = (Target *)func_0017D3D0((*nodes[i]->flags >> 7) & 0x7FF);
        if (candidate) {
            Vec *from = &self->body->pos;
            Vec *to = &nodes[i]->pos;
            sub(&diff, to, from);
            __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                              : "=&f"(length) : "f"(diff.x), "f"(diff.y), "f"(diff.z));
            if (length < distance) {
                distance = length;
                best = candidate;
            }
        }
    }
    if (best) {
        Body *body;
        success = 1;
        *found = best;
        body = best->v3();
        sub(&diff, &body->pos, &self->body->pos);
        func_00221888(&self->body->x40, &diff, out);
        norm(out, out);
    }
    return success;
}
