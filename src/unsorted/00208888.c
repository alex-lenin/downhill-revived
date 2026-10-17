/* UNIT 0x00208888 0x002088D0 */
extern float fabsf(float);
void func_00208888(float *value, float limit) {
    float current = *value;
    if (limit < fabsf(current)) {
        if (current > 0.0f) *value = limit;
        else *value = -limit;
    }
}
