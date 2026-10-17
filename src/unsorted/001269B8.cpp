/* UNIT 0x001269B8 0x00126B20 */
typedef unsigned int u128 __attribute__((mode(TI)));

struct Vec4 {
    union {
        u128 q;
        float f[4];
    };
};

struct Body {
    char pad0[0x10];
    Vec4 pos;
    char pad20[0x40 - 0x20];
    char x40[0x10];
    Vec4 x50;
    char pad60[0xA0 - 0x60];
    char xA0[1];
};

class Actor {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual Body *v4();
};

struct Obj {
    char pad0[0x130];
    Actor *x130;
    char pad134[0x1D0 - 0x134];
    Vec4 target;
    char pad1E0[0x200 - 0x1E0];
    Vec4 x200;
};

extern "C" void func_002226C8(void *, Vec4 *);
extern "C" void func_00222460(void *, void *);
extern "C" void func_00123DB0(Obj *, void *);

/* 128-bit vector copy (lq/sq through $2) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"((dst).q) : "r"(&(src)) : "$2")

/* copies xyz, keeps dst's w */
static inline void SetXYZ(Vec4 *dst, Vec4 *src)
{
    float w = dst->f[3];

    COPY_VEC(*dst, *src);
    dst->f[3] = w;
}

extern "C" void func_001269B8(Obj *obj)
{
    Vec4 dir;
    float length;
    float inv;
    float x;
    float y;
    float z;

    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(dir) : "r"(&obj->target), "r"(&obj->x130->v4()->pos));
    x = dir.f[0];
    y = dir.f[1];
    z = dir.f[2];
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %2, %0, %0\n\tsqrt.s %2, %2\n\tlui $2,0x3f80\n\tmtc1 $2,%1\n\tnop\n\tdiv.s %1,%1,%2"
                      : "=f"(z), "=&f"(inv), "=&f"(length) : "f"(x), "f"(y), "0"(z) : "$2");
    dir.f[0] = x * inv;
    dir.f[1] = y * inv;
    dir.f[2] = z * inv;
    if (length > 0.01f) {
        func_002226C8(obj->x130->v4()->x40, &dir);
        func_00222460(obj->x130->v4()->x40, obj->x130->v4()->xA0);
        SetXYZ(&obj->x200, &obj->x130->v4()->x50);
        func_00123DB0(obj, obj->x130->v4()->x40);
    }
}
