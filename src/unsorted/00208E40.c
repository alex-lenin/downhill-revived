/* UNIT 0x00208E40 0x00208EE0 */
extern float func_00222330(int, int);
int func_00208E40(int arg0, int arg1, int low, int high) {
 float value = func_00222330(arg0, arg1);
 if (value < (float)low) return 1024;
 if ((float)high < value) return 0;
 return (int)(((float)high - value) / (float)(high - low) * 1024.0f);
}
