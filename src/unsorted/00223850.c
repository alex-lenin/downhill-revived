/* UNIT 0x00223850 0x002238A8 */
extern float func_00223778(float x);
float func_00223850(float x) {
    float value = func_00223778(x) - 0.5f;
    if (value < -1.0f) value += 2.0f;
    return -value;
}
