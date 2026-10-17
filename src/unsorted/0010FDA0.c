/* UNIT 0x0010FDA0 0x0010FE50 */
typedef struct Rider {
    char pad0[0x240];
    float x240;
    char pad244[0xC];
    float x250;
    char pad254[0xC];
    float x260;
} Rider;

extern void func_0010B2B8(Rider *rider, void *item);

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

void func_0010FDA0(Rider *rider)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (Abs(rider->x250) < 0.17453292f) {
            break;
        }
        if (!(rider->x240 < 50.0f || Abs(rider->x250) > Abs(rider->x260))) {
            return;
        }
        func_0010B2B8(rider, 0);
    }
}
