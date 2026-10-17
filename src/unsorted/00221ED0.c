/* UNIT 0x00221ED0 0x00221F20 */
void func_00221ED0(float *dst, const float *src) {
    int i;
    for (i = 0; i < 3; i++) {
        float *d = (float *)((i << 4) + (int)dst);
        const float *s = (const float *)((i << 4) + (int)src);
        int count = 2;
        do {
            float value = *s;
            s++;
            *d = value;
            d++;
        } while (--count >= 0);
    }
}
