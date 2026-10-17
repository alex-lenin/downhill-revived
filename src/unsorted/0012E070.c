/* UNIT 0x0012E070 0x0012E158 .sdata=0x29C288:0x29C290 */
typedef struct Rider {
    char pad0[0x11B8];
    int x11B8;
    char pad11BC[0x1280 - 0x11BC];
    int x1280;
    char pad1284[0x13E1 - 0x1284];
    unsigned char x13E1;
    char pad13E2[0x40C0 - 0x13E2];
    char x40C0[0x7A6A - 0x40C0];
    unsigned char x7A6A;
    char pad7A6B[5];
    int x7A70;
} Rider;

float D_0029C288 = 45.0f;
float D_0029C28C = 10.0f;

extern int func_00138560(void *p);
extern int func_00238F48(void);

static inline float Clamp01(float x)
{
    float lo;
    float out;

    __asm__ volatile ("min.s %0, %1, %2" : "=&f"(lo) : "f"(1.0f), "f"(x));
    __asm__ volatile ("max.s %0, %1, %2" : "=&f"(out) : "f"(0.0f), "f"(lo));
    return out;
}

static inline float SmoothStep(float t)
{
    return (t * -2.0f + 3.0f) * t * t;
}

float func_0012E070(Rider *r)
{
    float result;
    int active;
    int elapsed;

    if ((unsigned char)func_00138560(r->x40C0)) {
        return 0.0f;
    }
    result = 0.0f;
    if (r->x7A6A) {
        active = r->x13E1 ? r->x1280 : r->x11B8;
        if (active) {
            result = 1.0f;
        } else {
            elapsed = func_00238F48() - r->x7A70;
            result = elapsed;
            result -= D_0029C28C;
            result = Clamp01(result / D_0029C288);
            result = SmoothStep(result);
        }
    }
    return result;
}
