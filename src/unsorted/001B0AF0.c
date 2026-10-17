/* UNIT 0x001B0AF0 0x001B0B90 .sdata=0x29D3E8:0x29D3F0 */
typedef struct {
    char pad0[0x44];
    float x44;
    char pad48[0x24];
    float x6C;
    float x70;
    unsigned char x74;
} Obj;

float D_0029D3E8 = 0.3f;
float D_0029D3EC = 0.1f;

extern float func_00236088(float, float, float, float);
extern float fabsf(float);

void func_001B0AF0(Obj *arg0)
{
    float diff;
    float adiff;

    if (arg0->x74 != 0) {
        arg0->x6C = func_00236088(arg0->x6C, arg0->x70, D_0029D3E8, 0.001f);
    } else {
        arg0->x6C = func_00236088(arg0->x6C, arg0->x70, D_0029D3EC, 0.001f);
    }
    diff = arg0->x6C - arg0->x70;
    adiff = fabsf(diff);
    /* Zero-byte label: scheduling barrier so the 0.0025f load stays after abs.s;
       keeping diff live here puts diff/abs in $f0/$f2 (cf. src/unsorted/0014D3B8.c). */
    __asm__ volatile ("Labsdiff:" : : "f" (adiff), "f" (diff));
    if (adiff < 0.0025f) {
        arg0->x70 = 0.0f;
        arg0->x74 = 0;
    }
    arg0->x44 = arg0->x6C;
}
