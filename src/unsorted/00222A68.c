/* UNIT 0x00222A68 0x00222B08 */
extern unsigned int func_00243198(void *);
extern char D_00663140[];
float func_00222A68(float min, float max) {
    float random;
    if(max <= min) return min;
    random = (float)(func_00243198(D_00663140)&0x3FFF);
    return min + (max-min)*random*0.00006103515625f;
}
