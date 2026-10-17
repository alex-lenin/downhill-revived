/* UNIT 0x001DCE58 0x001DD030 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    Vec pos;
    Vec vel;
    Vec dir;
    Vec force;
    float x40;
    float maxSpeed;
    char pad48[0x50 - 0x48];
    float maxForce;
    char pad54[0x58 - 0x54];
    float speed;
} Obj;

extern int func_00238F30(void);

static inline float VecNormalize(Vec *out, Vec *v)
{
    float x = v->x, y = v->y, z = v->z, length, inv;
    int bits;

    /* length and 1/length (libvu0 style) */
    __asm__ volatile ("mula.s %3, %3
	madda.s %4, %4
	madd.s %1, %5, %5
	sqrt.s %1, %1
	"
                      "lui %2, 0x3f80
	mtc1 %2, %0
	nop
	div.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(length), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return length;
}

static inline void VecScale(Vec *out, Vec *v, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "mfc1 $2, %2\n\t"
                      "qmtc2.ni $2, $vf12\n\t"
                      "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(v), "f"(s) : "$2");
}

static inline void VecCopy(Vec *out, Vec *v)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(v) : "$2", "memory");
}

static inline void VecAddScaled(Vec *out, Vec *a, Vec *b, float s)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "mfc1 $2, %3\n\t"
                      "qmtc2.ni $2, $vf13\n\t"
                      "vaddax.xyz ACC, $vf11, $vf0x\n\t"
                      "vmaddx.xyz $vf11, $vf12, $vf13x\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b), "f"(s) : "$2");
}

void func_001DCE58(Obj *obj)
{
    Vec tmp;
    Vec unit;
    float length;
    float dt;

    if (0.0f < obj->maxForce) {
        length = VecNormalize(&tmp, &obj->force);
        if (obj->maxForce < length) {
            VecScale(&obj->force, &tmp, obj->maxForce);
        }
    }
    VecScale(&tmp, &obj->force, obj->x40);
    VecScale(&obj->vel, &obj->dir, obj->speed);
    dt = 1.0f / (float)func_00238F30();
    VecAddScaled(&obj->vel, &obj->vel, &tmp, dt);
    obj->speed = VecNormalize(&unit, &obj->vel);
    if (1e-10f < obj->speed) {
        VecCopy(&obj->dir, &unit);
        if (obj->maxSpeed < obj->speed) {
            obj->speed = obj->maxSpeed;
            VecScale(&obj->vel, &obj->dir, obj->speed);
        }
        VecAddScaled(&obj->pos, &obj->pos, &obj->vel, dt);
    }
}
