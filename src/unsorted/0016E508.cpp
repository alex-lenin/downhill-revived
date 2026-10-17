/* UNIT 0x0016E508 0x0016E5D8 */
typedef struct Vec4 { float x, y, z, w; } __attribute__((aligned(16))) Vec4;

struct Body {
    char pad0[0x10];
    Vec4 pos;
};

class Target {
public:
    virtual void v08();
    virtual void v10();
    virtual void v18();
    virtual Body *GetBody();          /* vtable +0x20 */
};

struct Self {
    char pad0[0x200];
    Body *body;
};

struct Other {
    char pad0[0x130];
    Target *target;
};

extern void func_0016E5D8(Self *self, Other *other, float t);

int func_0016E508(Self *self, Other *other, float t)
{
    Vec4 *a;
    Vec4 *b;
    Vec4 d;
    float dist;
    int hit;

    hit = 0;
    a = &self->body->pos;
    b = other->target ? &other->target->GetBody()->pos : 0;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(&d), "r"(a), "r"(b) : "memory");
    /* horizontal (x/y) distance */
    __asm__ volatile ("mula.s %1,%1\n\tmadd.s %0,%2,%2\n\tsqrt.s %0,%0" : "=&f"(dist) : "f"(d.x), "f"(d.y));
    if (dist < 3.0f) {
        hit = 1;
        func_0016E5D8(self, other, t);
    }
    return hit;
}
