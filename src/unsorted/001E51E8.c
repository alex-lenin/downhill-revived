/* UNIT 0x001E51E8 0x001E5228 .sdata=0x29D784:0x29D788 */
float D_0029D784 = 0.5f;
float func_001E51E8(void *obj, int value) {
    float result = D_0029D784 * (float)value + 0.5f;
    if (result < 1.0f) result = 1.0f;
    return result;
}
