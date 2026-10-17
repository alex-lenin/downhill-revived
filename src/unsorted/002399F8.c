/* UNIT 0x002399F8 0x00239A58 */
extern int D_0027EC68[], D_0027EC78[];
extern float D_0027ECA4[];
extern float D_0075A690[];
void func_002399F8(unsigned int index) {
    if (index < 4) {
        int *state = &D_0027EC68[index];
        if (*state < 3) {
            D_0075A690[index] = D_0027ECA4[0];
            *state = 3;
            D_0027EC78[index] = 0;
        }
    }
}
