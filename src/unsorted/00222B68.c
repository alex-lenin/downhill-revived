/* UNIT 0x00222B68 0x00222BD8 */
extern int func_002559D8(void);

/* Random float in [min, max) */
float func_00222B68(float min, float max)
{
    if (max <= min) {
        return min;
    }
    return min + (max - min) * (float)(func_002559D8() & 0x3FFF) * (1.0f / 16384.0f);
}
