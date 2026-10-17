/* UNIT 0x00236558 0x00236700 */
typedef float Vector[4];
typedef struct Obj { Vector x0, x10, x20; float x30, x34, x38, x3c; } Obj;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern void func_00236438(float *a, float *b, float *out, float positive, float tangent, float negative);
extern void func_00222F60(float *quat, float *from, float *to);
extern float func_002237F8(float value);
extern void func_00223168(float *out, float *in);
extern void func_002235B0(float *out, float *q, float *v);

static __inline__ float Normalize(float *out, float *v) {
    float x = v[0], y = v[1], z = v[2], len, inv;
    int bits;
    __asm__ volatile ("mula.s %3, %3\n\tmadda.s %4, %4\n\tmadd.s %1, %5, %5\n\tsqrt.s %1, %1\n\t"
                      "lui %2, 0x3f80\n\tmtc1 %2, %0\n\tnop\n\tdiv.s %0, %0, %1"
                      : "=&f"(inv), "=&f"(len), "=r"(bits) : "f"(x), "f"(y), "f"(z));
    out[0] = x * inv;
    out[1] = y * inv;
    out[2] = z * inv;
    return len;
}

void func_00236558(Obj *obj, float *input, float dt) {
    Vector tmp, sum, norm, rot;
    float length, w, limit;
    float *axis = obj->x20;
    float *vel;

    func_00236438(input, axis, tmp, obj->x34 * dt, obj->x38 * dt, obj->x3c * dt);
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(sum) : "r"(obj->x10), "r"(tmp) : "memory");
    length = Normalize(norm, sum);
    if (0.0f < length) {
        func_00222F60(rot, norm, axis);
        limit = func_002237F8(obj->x30 * dt * 0.5f);
        if (rot[3] < limit) {
            rot[3] = limit;
            func_00223168(rot, rot);
            func_002235B0(norm, rot, axis);
        }
        w = axis[3];
        COPY_VECTOR(*(Vector *)axis, norm);
        axis[3] = w;
    }
    func_00236438(sum, obj->x20, sum, obj->x0[3], obj->x10[3], obj->x20[3]);
    vel = obj->x10;
    w = vel[3];
    COPY_VECTOR(*(Vector *)vel, sum);
    vel[3] = w;
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\t"
                      "vaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0"
                      : "=m"(rot) : "r"(obj->x0), "r"(vel), "f"(dt) : "$2", "memory");
    w = obj->x0[3];
    COPY_VECTOR(obj->x0, rot);
    obj->x0[3] = w;
}
