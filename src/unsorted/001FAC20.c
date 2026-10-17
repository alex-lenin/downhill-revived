/* UNIT 0x001FAC20 0x001FAC50 */
typedef struct Obj {
    char pad0[0x3C];
    float x3c;
} Obj;

static inline float fmin_s(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float fmax_s(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

float func_001FAC20(Obj *obj)
{
    return fmax_s(fmin_s(obj->x3c, 1.0f), 0.85f);
}
