/* UNIT 0x00222E80 0x00222EE8 */
void func_00222E80(float *out, const float *axis, float angle) {
    float values[2];
    float sine, cosine;
    angle *= 0.5f;
    __asm__ volatile (
        "mfc1 $2,%2\n\t"
        "qmtc2 $2,$vf14\n\t"
        "vcallms 0x978\n\t"
        "nop\n\t"
        "qmfc2.i $2,$vf14\n\t"
        "mtc1 $2,%0\n\t"
        "qmfc2 $2,$vf15\n\t"
        "mtc1 $2,%1"
        : "=&f"(sine), "=&f"(cosine) : "f"(angle) : "$2");
    values[0] = sine;
    values[1] = cosine;
    out[0] = values[0] * axis[0];
    out[1] = values[0] * axis[1];
    out[2] = values[0] * axis[2];
    out[3] = values[1];
}
