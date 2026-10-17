/* UNIT 0x00235F38 0x00236088 */
extern void func_002087C0(float *angle);

float func_00235F38(float cur, float target, float rate, float minStep)
{
    float delta;
    float result;
    float step;

    if (cur == target) {
        result = target;
    } else {
        delta = target - cur;
        func_002087C0(&delta);
        step = delta * rate;
        if (fabsf(step) < minStep) {
            if (step < 0.0f) {
                step = -minStep;
            } else {
                step = minStep;
            }
        }
        if ((cur > 1.5707964f && target < -1.5707964f) ||
            (cur < -1.5707964f && target > 1.5707964f)) {
            target = cur + delta;
        }
        result = cur + step;
        if (cur < target) {
            if (result > target) {
                result = target;
            }
        } else if (result < target) {
            result = target;
        }
        func_002087C0(&result);
    }
    return result;
}
