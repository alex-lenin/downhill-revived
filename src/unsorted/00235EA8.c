/* UNIT 0x00235EA8 0x00235F38 */
extern float fabsf(float);
float func_00235EA8(float current, float target, float rate, float minimum) {
    float step, result;
    if (current == target) return target;
    step = (target-current)*rate;
    if (fabsf(step) < minimum) {
        if (step < 0.0f) step = -minimum;
        else step = minimum;
    }
    result = current + step;
    if (current < target) {
        if (target < result) return target;
    } else if (result < target) result = target;
    return result;
}
