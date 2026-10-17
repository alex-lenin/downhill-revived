/* UNIT 0x001FAFF8 0x001FB160 */
typedef struct Part { char pad[0x354]; unsigned char x354; char pad355[0x7f]; unsigned char x3D4; } Part;
typedef struct Obj { char pad[0x40c0]; Part x40C0; char pad4495[0xa37]; void *x4ECC; } Obj;
typedef struct Arg { Obj *x0; char pad4[4]; float x8; char padc[0x10]; float x1C; char pad20[8]; int x28; } Arg;
extern float func_00236088(float, float, float, float);
extern int func_001FAD78(Arg *, int);
extern void func_001FB468(Arg *);
int func_001FAFF8(Arg *arg0) {
 float target, step, rate;
 Obj *obj;
 Part *part;
 if (arg0->x0->x4ECC == arg0) return 0;
 {
  arg0->x8 = func_00236088(arg0->x8, 0.0f, 0.03f, 0.02f);
  obj = arg0->x0;
  part = &obj->x40C0;
  if (arg0->x28 == 0 && obj->x40C0.x3D4 != 2) {
   arg0->x1C = func_00236088(arg0->x1C, -0.1f, 0.006f, 0.003f);
  } else if (arg0->x28 == 1 && part->x354 != 2) {
   arg0->x1C = func_00236088(arg0->x1C, -0.1f, 0.006f, 0.003f);
  } else {
   arg0->x1C = func_00236088(arg0->x1C, 0.0f, 0.012f, 0.006f);
  }
  if (func_001FAD78(arg0, arg0->x28)) {
   float value = arg0->x1C;
   float zero = 0.0f;
   __asm__("max.s %0,%1,%2" : "=f"(value) : "f"(value), "f"(zero));
   arg0->x1C = value;
  }
  func_001FB468(arg0);
  return arg0->x8 != 0.0f;
 }
}
