/* UNIT 0x0016AE18 0x0016AFB8 .sdata=0x29C888:0x29C898 */
typedef struct Vec4 { float x, y, z, w; } Vec4;
typedef struct Obj {
    int x0, x4;
    char pad8[0x30 - 0x8];
    Vec4 x30;
    Vec4 x40;
} Obj;
float D_0029C888=1.15f;
float D_0029C88C=3.0f;
float D_0029C890=20.0f;
float D_0029C894=40.0f;
extern float D_0026EC30[];
extern float func_00138580(int);
extern float func_00248B90(float);
extern float func_0016AD98(float *values, float t);

static inline float InnerProduct(Vec4 *a, Vec4 *b) {
    float result;
    __asm__ volatile ("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return result;
}

float func_0016AE18(Obj *obj) {
 int kind=obj->x0;
 float t,value,limited,clamped;
 switch(kind) {
 case 1:
  /* angle between the two vectors, scaled by 2/pi */
  t=func_00248B90(InnerProduct(&obj->x40,&obj->x30))*0.63661975f;
  if (t > 1.0f) t = 1.0f;
  return func_0016AD98(D_0026EC30,t);
 case 3: {
  value=(func_00138580(obj->x4)*0.6818181872367859f-D_0029C890)/(D_0029C894-D_0029C890);
  __asm__ volatile("min.s %0, %1, %2" : "=&f"(limited) : "f"(value), "f"(1.0f));
  __asm__ volatile("max.s %0, %1, %2" : "=&f"(clamped) : "f"(limited), "f"(0.0f));
  return D_0029C888*clamped+1.0f;
 }
 case 0: case 2: default: return 1.0f;
 }
}

float func_0016AF28(Obj *obj) {
 int kind=obj->x0;
 float value,limited,clamped;
 switch(kind) {
 case 3: {
  value=(func_00138580(obj->x4)*0.6818181872367859f-D_0029C890)/(D_0029C894-D_0029C890);
  __asm__ volatile("min.s %0, %1, %2" : "=&f"(limited) : "f"(value), "f"(1.0f));
  __asm__ volatile("max.s %0, %1, %2" : "=&f"(clamped) : "f"(limited), "f"(0.0f));
  return D_0029C88C*clamped+1.0f;
 }
 case 0: case 1: case 2: default: return 1.0f;
 }
}
