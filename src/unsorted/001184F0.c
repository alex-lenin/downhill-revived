/* UNIT 0x001184F0 0x00118588 */
extern int func_001190B0(void *);
float func_001184F0(unsigned char *obj) {
    float result = 0.0f;
    int frames;
    if (obj[0x1089F]) {
        frames = func_001190B0(obj) / 60;
        result = (float)((frames / 60) % 60) + (float)(frames % 60) * 0.01f;
    }
    return result;
}
