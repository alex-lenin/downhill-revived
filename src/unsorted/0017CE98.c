/* UNIT 0x0017CE98 0x0017CF98 .sdata=0x29CA98:0x29CAA8 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Node {
    char pad0[0x10];
    Vec4 pos;
    char pad20[0x20];
    char x40[1];
} Node;

typedef struct Spring {
    char pad0[0xC];
    float target;
    char pad10[0x4];
    Node *node;
    char pad18[0x10];
    float vel;
} Spring;

/* 128-bit vector copy (lq/sq through $2) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

float D_0029CA98 = 3.0f;
float D_0029CA9C = -2.0f;
volatile float D_0029CAA0 = 0.025f;
volatile float D_0029CAA4 = 0.125f;

extern float fabsf(float);
#define Abs fabsf

extern float func_00238F40(void);
extern void func_0017CD20(Spring *spring);
extern void func_00209248(Node *node, Vec4 *pos, void *x40, float dt);

int func_0017CE98(Spring *spring)
{
    Vec4 pos;
    float accel;
    float dt;
    float *vel;
    float v;
    float newVel;
    float newZ;
    float mag;

    if (spring->node == 0) {
        return 0;
    }
    COPY_VEC(pos, &spring->node->pos);
    accel = D_0029CA98 * (spring->target - pos.z) + D_0029CA9C * spring->vel;
    dt = func_00238F40();
    accel *= dt;
    vel = &spring->vel;
    v = *vel;
    newVel = v + accel;
    newZ = pos.z + v * dt;
    *vel = newVel;
    pos.z = newZ;
    __asm__ volatile ("" : : : "memory");
    mag = Abs(newVel);
    __asm__ volatile ("" : : "f"(newVel), "f"(mag));
    if (mag < D_0029CAA0) {
        float limit;
        mag = Abs(newZ - spring->target);
        __asm__ volatile ("" : : : "memory");
        limit = D_0029CAA4;
        __asm__ volatile ("" : "+f"(limit) : "f"(mag) : "$f0");
        if (mag < limit) {
            func_0017CD20(spring);
            return 0;
        }
    }
    func_00209248(spring->node, &pos, spring->node->x40, func_00238F40());
    return 1;
}
