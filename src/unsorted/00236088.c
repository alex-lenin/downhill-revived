/* UNIT 0x00236088 0x00236130 */
float func_00235EA8(float, float, float, float);                /* extern */
float func_00238F38();                                /* extern */
float func_00248FF8(float, float);                        /* extern */

float func_00236088(float fparg0, float fparg1, float fparg2, float fparg3) {
    float temp_f20;
    float var_f21;
    float var_f22;

    var_f22 = fparg3;
    var_f21 = fparg2;
    temp_f20 = 60.0f / func_00238F38();
    if (temp_f20 > 1.0f) {
        var_f21 = 1.0f - func_00248FF8(1.0f - var_f21, temp_f20);
        var_f22 *= temp_f20;
    }
    return func_00235EA8(fparg0, fparg1, var_f21, var_f22);
}
