/* UNIT 0x00133518 0x001336F8 */
typedef struct Spring {
    char pad0[0x60];
    float vel;
    float stiffness;
    float dampNeg;
    float dampPos;
} Spring;

typedef struct Obj {
    char pad0[0x2E0];
    Spring a;
    char pad350[0x10];
    Spring b;
} Obj;

extern int D_0029C2F0;
extern void func_002327B0(Obj *obj, float dt);
extern float func_00134E40(Spring *spring);
extern void func_00134E88(Spring *spring, float value);

static inline float Max(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float Min(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

/* Keeps value in [0, 1]; at a limit the velocity may only point back inside. */
static inline void UpdateSpring(Spring *spring, float dt)
{
    float value;
    float accel;

    if (D_0029C2F0 != 0) {
        value = func_00134E40(spring);
        if (value <= 0.0f) {
            value = 0.0f;
            spring->vel = Max(value, spring->vel);
        } else if (value >= 1.0f) {
            value = 1.0f;
            spring->vel = Min(0.0f, spring->vel);
        }
        accel = spring->stiffness * (1.0f - value);
        accel -= (spring->vel < 0.0f ? spring->dampNeg : spring->dampPos) * spring->vel;
        func_00134E88(spring, value + spring->vel * dt);
        spring->vel += accel * dt;
    }
}

void func_00133518(Obj *obj, float dt)
{
    func_002327B0(obj, dt);
    UpdateSpring(&obj->a, dt);
    UpdateSpring(&obj->b, dt);
}
