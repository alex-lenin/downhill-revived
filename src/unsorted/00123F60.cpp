/* UNIT 0x00123F60 0x00124300 */
struct Vec { float x, y, z, w; } __attribute__((aligned(16)));
struct Mat { Vec v[4]; };

struct Vec4 {
    float x, y, z, w;
    Vec4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

struct Target {
    char pad0[0x4];
    char x4;
    char pad5[0xB];
    Vec x10;
    char pad20[0x20];
    Mat x40;
};

struct ModelHolder {
    char pad0[0x8];
    Target *x8;
};

class Model {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Target *v3();
};

struct Obj {
    char pad0[0x4];
    int active;                 /* 0x4 */
    char pad8[0x28];
    float x30;
    char pad34[0x34];
    char x68[0xA8];
    Vec x110;
    char pad120[0x10];
    Model *model;               /* 0x130 */
    void *x134;                 /* 0x134 */
    char pad138[0xA8];
    Vec x1E0;
    char pad1F0[0x10];
    Vec x200;
};

extern char D_0038DCE0[];
extern const float D_0029C1C8[];
extern "C" {
void func_00127458(Obj *obj);
float func_00124960(Obj *obj, Vec *pos, int force);
void func_001AC880(void *p, float value);
void *func_00164A10(void *p);
void func_002182A8(void *p, int a);
void func_00192B38(Model *model, void *out);
void func_00164EE8(void *p, int a, Target *b, Vec4 *v, void *buf);
float func_00235EA8(float, float, float, float);
}

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline void Cross(Vec *d, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(*d) : "r"(a), "r"(b));
}

/* 64-byte matrix copy (as sceVu0CopyMatrix) */
static inline void CopyMatrix(Mat *dst, Mat *src)
{
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t"
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3"
                      : "=m"(dst->v[0]), "=m"(dst->v[1]), "=m"(dst->v[2]), "=m"(dst->v[3])
                      : "r"(src) : "$8", "$9", "$10", "$11");
}

static inline void Norm(Vec *v)
{
    float x = v->x, y = v->y, z = v->z, len, inv;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3" : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    __asm__ volatile ("rsqrt.s %0,%1,%2" : "=&f"(inv) : "f"(1.0f), "f"(len));
    v->x = x * inv; v->y = y * inv; v->z = z * inv;
}

/* uses VU0/FPU inline asm */
extern "C" void func_00123F60(Obj *obj, int force)
{
    Vec pos;
    Target *target;
    Vec *cur;
    float drop, w;

    COPY_VECTOR(pos, &obj->x1E0);
    func_00127458(obj);
    drop = func_00124960(obj, &pos, force);
    target = ((ModelHolder *)obj->model)->x8;
    if (target != 0) {
        Mat m;
        COPY_VECTOR(m.v[2], &obj->x110);
        Norm(&m.v[2]);
        Cross(&m.v[0], &obj->x200, &m.v[2]);
        Norm(&m.v[0]);
        Cross(&m.v[1], &m.v[2], &m.v[0]);
        Norm(&m.v[1]);
        m.v[3].x = 0.0f;
        m.v[3].y = 0.0f;
        m.v[3].z = 0.0f;
        m.v[0].w = 0.0f;
        m.v[1].w = 0.0f;
        m.v[2].w = 0.0f;
        m.v[3].w = 1.0f;
        CopyMatrix(&target->x40, &m);
        if (drop == D_0029C1C8[0]) {
            target->x4 = 0;
        } else {
            target->x4 = 1;
            COPY_VECTOR(target->x10, &pos);
            target->x10.z -= drop;
        }
    }
    if (drop != D_0029C1C8[0]) {
        if (drop > 0.0f) {
            if (drop > 10.0f && !force) {
                char buf[0x30];
                void *p;

                func_001AC880(obj->x68, 0.0f);
                if (obj->x134 == 0) {
                    p = func_00164A10(D_0038DCE0);
                    obj->x134 = p;
                    if (p == 0) {
                        goto done;
                    }
                }
                obj->active = 1;
                if (((ModelHolder *)obj->model)->x8 != 0) {
                    func_002182A8(((ModelHolder *)obj->model)->x8, 0);
                }
                func_00192B38(obj->model, buf);
                Vec4 vec(2.0f, 6.0f, 0.0f, 0.0f);
                func_00164EE8(obj->x134, 0, obj->model->v3(), &vec, buf);
            } else if (drop > 3.0f) {
                goto settle;
            } else {
                obj->x30 = 0.0f;
                pos.z = func_00235EA8(pos.z, pos.z - drop, 0.15f, 0.001f);
            }
        } else {
            obj->x30 = 0.0f;
            pos.z -= drop;
        }
    } else {
settle:
        obj->x30 = func_00235EA8(obj->x30, 1.9555556f, 0.08f, 0.001f);
        pos.z -= obj->x30;
    }
done:
    cur = &obj->x1E0;
    w = cur->w;
    COPY_VECTOR(obj->x1E0, &pos);
    cur->w = w;
    COPY_VECTOR(obj->model->v3()->x10, &pos);
}
