/* UNIT 0x00232C98 0x00232D68 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Obj {
    char pad0[0x34];
    float *x34;
    float (*x38)[4];
} Obj;

extern void func_00222E80(float *quat, float *axis, float angle);
extern void func_002232C8(float (*out)[4], float *quat);
extern void func_00221D58(float (*out)[4], float (*in)[4]);
extern void func_00221A60(float (*out)[4], float (*a)[4], float (*b)[4]);
extern void func_002235B0(float *out, float *quat, float *v);

/* Rotates obj by angle about axis around pivot (orientation and position). */
void func_00232C98(Obj *obj, float *pivot, float *axis, float angle)
{
    float quat[4] __attribute__((aligned(16)));
    float rot[4][4] __attribute__((aligned(16)));
    float tmp[4][4] __attribute__((aligned(16)));
    float res[4][4] __attribute__((aligned(16)));
    Vec rel;
    float out[4] __attribute__((aligned(16)));

    func_00222E80(quat, axis, -angle);
    func_002232C8(rot, quat);
    func_00221D58(tmp, obj->x38);
    func_00221A60(res, rot, tmp);
    func_00221D58(obj->x38, res);
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(rel) : "r"(obj->x34), "r"(pivot) : "memory");
    func_002235B0(out, quat, &rel.x);
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvadd.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,0(%0)" : : "r"(obj->x34), "r"(pivot), "r"(out) : "memory");
}
