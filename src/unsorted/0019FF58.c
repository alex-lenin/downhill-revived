/* UNIT 0x0019FF58 0x001A0498 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Vec { float x, y, z, w; } Vec __attribute__((aligned(16)));
typedef struct Mtx { Vec row[4]; } Mtx;

typedef struct Obj {
    char pad0[0x10];
    int x10;
    char pad14[0x5C];
    Mtx m70;
    Vec xB0[12];
    Vec x170;
    Vec x180;
} Obj;

typedef struct Node {
    unsigned int flags;
    char pad4[0xC];
    Mtx m;
} Node;

extern void func_00221F50(Mtx *out, Vec *rot);
extern void func_00221A60(Mtx *out, Mtx *a, Mtx *b);
extern void func_00203590(Vec *v, int code, int a, int b);

static inline void ApplyMatrix(Vec *in, Mtx *m, Vec *out)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tlqc2 $vf13,0x10(%2)\n\tlqc2 $vf14,0x20(%2)\n\tvmulax.xyz ACC,$vf12,$vf11x\n\tvmadday.xyz ACC,$vf13,$vf11y\n\tvmaddz.xyz $vf11,$vf14,$vf11z\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(in), "r"(m));
}

static inline void VecAdd(Vec *out, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*out) : "r"(a), "r"(b));
}

void func_0019FF58(Obj *obj, Node *node)
{
    Vec v0, v1, c, d, rot, e, tmp;
    Mtx m2;
    unsigned int t;
    int count = 0;
    int half;
    int i;

    *(u128 *)&v0 = 0;
    *(u128 *)&v1 = 0;
    *(u128 *)&c = 0;
    *(u128 *)&d = 0;
    *(u128 *)&rot = 0;
    *(u128 *)&e = 0;
    v0.y = 10.0f;
    v0.z = 4.0f;
    v1.x = -26.45f;
    v1.y = 10.0f;
    v1.z = 4.0f;
    c.x = 5.0f;
    c.y = 3.0f;
    c.z = 4.0f;
    d.x = 5.0f;
    d.y = -3.0f;
    d.z = 4.0f;
    rot.x = -0.2617994f;
    rot.z = 1.5707964f;
    e.y = -30.0f;
    e.z = 30.0f;

    t = node->flags & 0xFFFC0000;
    if (t == 0x7DF00000) {
        count = 12;
    } else if (t == 0x7E100000) {
        count = 2;
    } else if (t == 0x7DF40000) {
        count = 4;
    } else if (t == 0x7DF80000 || t == 0x7DFC0000) {
        count = 6;
    } else if (t == 0x7E000000 || t == 0x7E040000 || t == 0x7E080000) {
        rot.x = -0.2841396f;
        d.x = -3.0f;
        c.x = 3.0f;
        rot.z = 0.0f;
        d.y = 9.7f;
        c.y = 9.7f;
        if ((node->flags & 0xFFFC0000) == 0x7E000000) {
            count = 8;
        } else {
            count = ((node->flags & 0xFFFC0000) == 0x7E080000) ? 10 : 6;
        }
    }

    half = ((unsigned int)count) >> 1;

    if (count == 2) {
        if ((node->flags & 0x3F) == 3) {
            obj->x10 = 1;
            rot.x = -0.2841396f;
            rot.z = 0.0f;
            VecAdd(&obj->xB0[0], &node->m.row[0], &v1);
            VecAdd(&obj->xB0[1], &node->m.row[0], &v0);
            func_00221F50(&obj->m70, &rot);
            ApplyMatrix(&e, &obj->m70, &tmp);
            VecAdd(&obj->x170, &tmp, &node->m.row[0]);
        }
    } else if ((node->flags & 0x3F) == 3) {
        obj->x10 = 1;
        for (i = 0; i < count; i++) {
            Vec *p = (i < half) ? &c : &d;
            VecAdd(&obj->xB0[i], &node->m.row[0], p);
            if ((node->flags & 0xFFFC0000) == 0x7E000000 ||
                (node->flags & 0xFFFC0000) == 0x7E040000 ||
                (node->flags & 0xFFFC0000) == 0x7E080000) {
                if (i < half) p->x += 6.0f; else p->x -= 6.0f;
            } else {
                if (i < half) p->y += 6.0f; else p->y -= 6.0f;
            }
        }
        func_00221F50(&obj->m70, &rot);
        ApplyMatrix(&e, &obj->m70, &tmp);
        VecAdd(&obj->x170, &tmp, &node->m.row[0]);
    } else if ((node->flags & 0x3F) == 4) {
        obj->x10 = 1;
        for (i = 0; i < count; i++) {
            Vec *p = (i < half) ? &c : &d;
            ApplyMatrix(p, &node->m, &tmp);
            VecAdd(&obj->xB0[i], &node->m.row[3], &tmp);
            if ((node->flags & 0xFFFC0000) == 0x7E000000 ||
                (node->flags & 0xFFFC0000) == 0x7E040000 ||
                (node->flags & 0xFFFC0000) == 0x7E080000) {
                if (i < half) p->x += 6.0f; else p->x -= 6.0f;
            } else {
                if (i < half) p->y += 6.0f; else p->y -= 6.0f;
            }
        }
        func_00221F50(&m2, &rot);
        func_00221A60(&obj->m70, &m2, &node->m);
        ApplyMatrix(&e, &obj->m70, &tmp);
        VecAdd(&obj->x170, &tmp, &node->m.row[3]);
    }

    func_00203590(&obj->x180, 0x1F86, 0, 0);
}
