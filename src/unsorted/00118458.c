/* UNIT 0x00118458 0x001184F0 */
extern int func_00119040(void *);
float func_00118458(unsigned char *obj) {
    float result = 0.0f;
    int frames;
    if (obj[0x1089F]) {
        frames = func_00119040(obj) / 60;
        result = (float)((frames / 60) % 60) + (float)(frames % 60) * 0.01f;
    }
    return result;
}
