/* UNIT 0x001DD030 0x001DD218 */
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

typedef struct Basis {
    Vec side;
    Vec front;
    Vec up;
} Basis;

extern void func_002227A8(Vec *dir, Vec *ref, Vec *outA, Vec *outB);

static inline float VecNormalize(Vec *out, Vec *v)
{
    float x = v->x;
    float y = v->y;
    float z = v->z;
    float length;
    float inv;
    int one;

    __asm__ volatile ("mula.s %3,%3
	madda.s %4,%4
	madd.s %1,%5,%5
	sqrt.s %1,%1
	"
                      "lui %2,0x3f80
	mtc1 %2,%0
	nop
	div.s %0,%0,%1"
                      : "=&f"(inv), "=&f"(length), "=r"(one) : "f"(x), "f"(y), "f"(z));
    out->x = x * inv;
    out->y = y * inv;
    out->z = z * inv;
    return length;
}

static inline void VecCopy(Vec *out, Vec *v)
{
    __asm__ volatile ("lq $2, 0x0(%1)
	sq $2, %0" : "=m"(*out) : "r"(v) : "$2", "memory");
}

static inline void VecCross(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                      "lqc2 $vf12, 0x0(%2)\n\t"
                      "vopmula.xyz ACC, $vf11, $vf12\n\t"
                      "vopmsub.xyz $vf11, $vf12, $vf11\n\t"
                      "sqc2 $vf11, %0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_001DD030(Obj *obj, Basis *basis, Vec *ref, int keepUp)
{
    Vec up = { 0.0f, 0.0f, 1.0f, 0.0f };
    Vec a;
    Vec b;

    if (ref != 0) {
        if (1e-10f < obj->speed) {
            func_002227A8(&obj->vel, ref, &a, &b);
        } else {
            func_002227A8(&obj->dir, ref, &a, &b);
        }
        VecNormalize(&basis->front, &a);
        if (keepUp == 0) {
            VecCopy(&up, ref);
        }
    } else if (1e-10f < obj->speed) {
        VecNormalize(&basis->front, &obj->vel);
    } else {
        VecNormalize(&basis->front, &obj->dir);
    }
    VecCross(&basis->side, &basis->front, &up);
    VecNormalize(&basis->side, &basis->side);
    VecCross(&basis->up, &basis->side, &basis->front);
}
