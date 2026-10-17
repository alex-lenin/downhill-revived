/* UNIT 0x002225A8 0x00222610 */
extern float func_00248D90(float, float);
void func_002225A8(float *out, const float *source)
{
    float length, squared;
    squared = source[0] * source[0] + source[1] * source[1];
    __asm__ volatile ("sqrt.s %0, %1" : "=&f"(length) : "f"(squared) : "memory");
    out[0] = func_00248D90(source[2], length);
    out[1] = 0.0f;
    out[2] = func_00248D90(source[0], source[1]);
}
