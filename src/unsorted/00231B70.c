/* UNIT 0x00231B70 0x00231BD8 */
extern void func_002319C8(float *, float, float, float);
void func_00231B70(float *p, float radius, float mass) {
    float v = (mass + mass) * radius * radius * 0.2f;
    func_002319C8(p, v, v, v);
    *p = 1.0f / mass;
}
