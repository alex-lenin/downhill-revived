/* UNIT 0x00223168 0x00223210 */
void func_00223168(float *out, float *in) {
    float x = in[0];
    float y = in[1];
    float z = in[2];
    float len_sq;
    float scale;

    len_sq = x * x + y * y;
    len_sq += z * z;
    if (len_sq < 1e-7f) {
        out[0] = 0.0f;
        out[1] = 0.0f;
        __asm__ volatile ("sqrt.s %0,%1" : "=&f"(len_sq) : "f"(1.0f - in[3] * in[3]));
        out[2] = len_sq;
    } else {
        __asm__ volatile ("sqrt.s %0,%1" : "=&f"(scale) : "f"((1.0f - in[3] * in[3]) / len_sq));
        out[0] = x * scale;
        out[1] = in[1] * scale;
        out[2] = in[2] * scale;
    }
    out[3] = in[3];
}
