/* UNIT 0x0016AD98 0x0016AE10 */
float func_0016AD98(float *values, float t) {
 if (t >= 1.0f) return values[4];
 else {
  int index=(int)(t*4.0f);
  float base=values[index];
  t -= (float)index * 0.25f;
  t *= 4.0f;
  return base + t * (values[index+1] - base);
 }
}
