/* UNIT 0x00117A10 0x00117A78 */
typedef struct Values { float x0, x4, x8; } Values;
int func_00117A10(Values *values, float value) {
 int result = 0;
 if (values->x8 != 0.0f) {
  result = (int)((value - values->x4 + values->x8 + 0.1f) / values->x8);
  if (result < 0) result = 0;
  else if (result >= 1024) result = 1023;
 }
 return result;
}
