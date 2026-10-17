/* UNIT 0x001DCD60 0x001DCDC8 .sdata=0x29D638:0x29D648 */
int D_0029D638 = 0;
float D_0029D63C = 10.0f;
float D_0029D640 = 20.0f;
int D_0029D644 = 0;

extern float D_004C1F20[][4];
extern float D_004C2020[][4];
extern void func_00222460(void *obj, float *out);

int func_001DCD60(float *src, void *obj)
{
    if (D_0029D638 == 16) {
        return 0;
    }
    /* quadword copy, same inline-asm idiom as the other lq/sq copies in this game */
    __asm__ volatile ("lq $2, 0x0(%0)\n\tsq $2, 0x0(%1)" : : "r"(src), "r"(D_004C1F20[D_0029D638]) : "$2");
    func_00222460(obj, D_004C2020[D_0029D638]);
    D_0029D638++;
    return 1;
}
