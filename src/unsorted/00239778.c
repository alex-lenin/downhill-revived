/* UNIT 0x00239778 0x00239858 */
extern int D_0027EC68[];
extern float D_0027EC78[];
extern float D_0027EC9C[];
extern float D_0075A690[];
extern int func_00238F60(void);

void func_00239778(int index)
{
    float *value;

    switch (D_0027EC68[index]) {
    case 1:
        break;
    case 2:
        value = &D_0075A690[index];
        if (*value > D_0027EC9C[0]) {
            *value -= func_00238F60() * 0.03333333f;
        }
        break;
    case 3:
        D_0027EC68[index] = 0;
        D_0075A690[index] = 0.0f;
        D_0027EC78[index] = -10.0f;
        break;
    }
}
