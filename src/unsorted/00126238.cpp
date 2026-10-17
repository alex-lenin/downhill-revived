/* UNIT 0x00126238 0x001263C0 */
#include "animation.h"
typedef struct Vec4 { float x, y, z, w; } Vec4 __attribute__((aligned(16)));

struct Transform {
    char pad0[0x40];
    char x40[0x40];
};

typedef AnimationHandle Name;

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};

/* the model's 16-byte name entries follow its 16-byte header */
class Model : public Part {
public:
    char pad4[0xC];
};

struct Body {
    char pad0[0x10];
    Vec4 x10;
};

struct Rider {
    char pad0[0x792C];
    Body *x792C;
};

struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x34];
    int x40[(0x130 - 0x40) / 4];
    Model *x130;
    char pad134[0x1D0 - 0x134];
    Vec4 x1D0;
    Vec4 x1E0;
    Vec4 x1F0;
};

extern "C" {
extern Rider *D_0043EEC0[];
void func_001258A0(Obj *obj, float dt);

float *func_00261020(Vec4 *v);
void func_002361D8(Vec4 *a, Vec4 *b, Vec4 *c, Vec4 *out);
float func_00238F40(void);
void func_00236558(void *a, void *b, float t);
void func_00123F60(Obj *obj, int a);
void func_00123DB0(Obj *obj, void *m);
void func_00211280(Transform *t);
}

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

/* copies xyz, keeps the destination's w */
static inline void CopyXYZ(Vec4 *dst, Vec4 *src)
{
    float w = dst->w;

    COPY_VECTOR(*dst, src);
    dst->w = w;
}

extern "C" void func_00126238(Obj *obj)
{
    Vec4 *pos;
    Vec4 *from;
    Vec4 dir;
    Rider *rider = D_0043EEC0[0];
    float speed;
    int offset;

    if (rider != 0) {
        speed = 23.466667f;
        pos = &obj->x1D0;
        func_001258A0(obj, speed);
        offset = obj->x40[obj->x8] * 16 + 16;
        { AnimationHandle copy = *(AnimationHandle *)((char *)obj->x130 + offset); func_00203E10(&copy, 0.8f); }
        CopyXYZ(pos, &rider->x792C->x10);
        *func_00261020(pos) = speed;
        func_002361D8(pos, &obj->x1E0, &obj->x1F0, &dir);
        from = &obj->x1E0;
    } else {
        __asm__ volatile ("lqc2 $vf11, 0(%0)\n\tmfc1 $2, %1\n\tqmtc2.ni $2, $vf12\n\tvmulx.xyz $vf11, $vf11, $vf12x\n\tsqc2 $vf11, 0(%2)"
            : : "r" (&obj->x1F0), "f" (-1.0f), "r" (&dir) : "$2", "memory");
        from = &obj->x1E0;
    }
    func_00236558(from, &dir, func_00238F40());
    func_00123DB0(obj, obj->x130->GetTransform()->x40);
    func_00123F60(obj, 0);
    func_00211280(obj->x130->GetTransform());
}
