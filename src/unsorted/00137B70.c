/* UNIT 0x00137B70 0x00137C78 */
typedef struct Obj {
    char pad0[0x158];
    void *x158;
    char pad15C[0x240 - 0x15C];
    float x240;
    char pad244[0x450 - 0x244];
    float x450;
    float x454;
    char pad458[0x470 - 0x458];
    float x470;
    char pad474[0x498 - 0x474];
    float x498;
} Obj;

extern int func_00138560(void *p);
extern float func_0012DFD0(void *p);
extern int func_001E64C8(void *p);
extern int func_001E7BD8(void *p);
extern float fabsf(float);


static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

void func_00137B70(Obj *obj)
{
    float pitchInput;
    float input;
    float limit;

    if (func_00138560(obj) == 0) {
        obj->x240 = 0.0f;
        return;
    }
    pitchInput = -func_0012DFD0(obj->x158);
    if (func_001E64C8((char *)obj->x158 + 0x6610) && !func_001E7BD8((char *)obj->x158 + 0x5FF0))
        pitchInput = 0.0f;
    input = func_0012DFD0(obj->x158);
    limit = 0.0f;
    if (input != limit) {
        float scale = obj->x498;
        float rate = obj->x240 + obj->x454 * pitchInput * scale;
        float magnitude;
        obj->x240 = rate;
        magnitude = Abs(rate);
        limit = obj->x450;
        if (limit * scale < magnitude)
            obj->x240 = limit * pitchInput * scale;
    } else {
        float rate = obj->x240 * obj->x470;
        obj->x240 = rate;
        if (Abs(rate) < 0.00014544411f)
            obj->x240 = limit;
    }
}
