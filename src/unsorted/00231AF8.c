/* UNIT 0x00231AF8 0x00231B70 */
extern void func_002319C8(float *, float, float, float);
void func_00231AF8(float *obj, float x, float y, float z, float mass) {
    float xx, yy, zz, factor;
    xx = x*x;
    yy = y*y;
    zz = z*z;
    factor = mass * 0.083333336f;
    func_002319C8(obj, (yy+zz)*factor, (xx+zz)*factor, (xx+yy)*factor);
    *obj = 1.0f / mass;
}
