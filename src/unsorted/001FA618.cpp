/* UNIT 0x001FA618 0x001FA670 */
extern "C" {
extern float D_0029D984[], D_0029D988[];
float func_001FA618(float *values, int count, int index) {
    float hi = D_0029D984[0];
    float lo = D_0029D988[0];
    float *p;
    if (count > 0) {
        p = values;
        do {
            float v = *p++;
            count--;
            hi = v >? hi;
            lo = v <? lo;
        } while (count);
    }
    return (values[index] - lo) / (hi - lo);
}
}
