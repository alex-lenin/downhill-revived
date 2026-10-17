/* UNIT 0x001272F8 0x001273B0 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Vec4 {
    union {
        u128 q;
        float f[4];
    };
};

struct Transform {
    char pad0[0x10];
    Vec4 x10;
};

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};

struct Obj {
    char pad0[0x120];
    Vec4 x120;
    Part *x130;
    char pad134[0x1E0 - 0x134];
    Vec4 x1E0;
};

extern "C" void func_00211280(Transform *t);

/* 128-bit vector copy (lq/sq through $2) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, %1\n\tsq $2, %0" : "=m"((dst).q) : "m"((src).q) : "$2")

/* sets the position, keeping x1E0's w component */
extern "C" void func_001272F8(Obj *obj, Vec4 *pos)
{
    if (obj->x130 != 0 && obj->x130->GetTransform() != 0) {
        Vec4 *v;
        float w;

        COPY_VEC(obj->x130->GetTransform()->x10, *pos);
        COPY_VEC(obj->x120, *pos);
        v = &obj->x1E0;
        w = v->f[3];
        COPY_VEC(*v, *pos);
        v->f[3] = w;
        func_00211280(obj->x130->GetTransform());
    }
}
