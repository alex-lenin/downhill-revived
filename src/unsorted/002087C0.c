/* UNIT 0x002087C0 0x00208888 */
extern float fabsf(float);

/* Wraps an angle into [-pi, pi). The do/while(0) block is needed for the
   retail scheduling (abs.s before the pi constant load). */
void func_002087C0(float *angle) {
    float value = *angle;

    do {
        if (fabsf(value) < 3.1415927f) {
            break;
        }
        value *= 0.15915494f; /* 1 / (2 pi) */
        value -= (float)(int)value;
        if (value > 0.5f) {
            value -= 1.0f;
        } else if (value <= -0.5f) {
            value += 1.0f;
        }
        *angle = value * 6.2831855f;
    } while (0);
}
