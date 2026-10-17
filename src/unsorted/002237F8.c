/* UNIT 0x002237F8 0x00223848 */
extern float func_00223638(float);
float func_002237F8(float value) {
    value += 0.5f;
    if (value > 1.0f) value -= 2.0f;
    return func_00223638(value);
}
