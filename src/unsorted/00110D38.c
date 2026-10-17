/* UNIT 0x00110D38 0x00110DA0 */
typedef struct Obj { char *x0; char pad4[0x2B8]; float x2BC; char pad2C0[0x80]; float x340; } Obj;
extern float func_001E7C00(void *, int);
int func_00110D38(Obj *obj, int index) {
 void *p = obj->x0 + 0x5FF0;
 int result = 0;
 if (p) {
  float value = obj->x340;
  if (value + func_001E7C00(p, index) < obj->x2BC) result = 1;
 }
 return result;
}
