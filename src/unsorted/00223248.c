/* UNIT 0x00223248 0x002232C0 */
void func_00223248(float *out, float *q) {
    out[0] = 2.0f * (q[0]*q[2] + q[1]*q[3]);
    out[1] = 2.0f * (q[1]*q[2] - q[0]*q[3]);
    out[2] = 1.0f - 2.0f * (q[0]*q[0] + q[1]*q[1]);
}
