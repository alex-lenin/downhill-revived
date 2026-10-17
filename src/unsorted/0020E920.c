/* UNIT 0x0020E920 0x0020E950 */
void func_0020E920(float *arg0, float *arg1, float a, float b, float c) {
    float value = ((c - 1.0f) * (b * a)) / (a - b);
    *arg1 = value;
    *arg0 = 1.0f - (value / a);
}
