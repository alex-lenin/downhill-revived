/* UNIT 0x00222690 0x002226C8 */
extern float func_00248D90(float, float);
float func_00222690(float *v) {
    float length = v[0] * v[0] + v[1] * v[1];
    float root;
    __asm__ volatile ("sqrt.s %0, %1" : "=&f" (root) : "f" (length));
    return func_00248D90(v[2], root);
}

