/* UNIT 0x001FAC50 0x001FACD8 */
extern float func_001FAC20(void *obj);
float func_001FAC50(void *obj) {
 float value = func_001FAC20(obj);
 if (value > 1.0f) return 1.0f;
 if (value < 0.85f) value = 0.0f;
 else value = (value - 0.85f) / (1.0f - 0.85f);
 return value;
}
