/* UNIT 0x00222D48 0x00222E10 */
extern float *func_0023D098(int index);
extern void *func_0023D688(int index);
extern void func_002218D0(void *camera, float *in, float *out);

void func_00222D48(int index, float *pos, float *screen)
{
    float tmp[4] __attribute__((aligned(16)));
    float res[4] __attribute__((aligned(16)));
    float *origin;
    void *camera;
    float inv;

    origin = func_0023D098(index);
    camera = func_0023D688(index);
    if (pos != 0 && screen != 0) {
        __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                          : "=m"(tmp) : "r"(pos), "r"(origin) : "memory");
        tmp[3] = 1.0f;
        func_002218D0(camera, tmp, res);
        inv = 1.0f / res[3];
        screen[0] = res[0] * inv;
        screen[1] = res[1] * inv;
        screen[2] = res[3];
        screen[3] = res[2] * inv;
    }
}
