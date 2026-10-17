/* UNIT 0x00172118 0x001722A0 */
typedef struct Value { int x[4]; } Value;
typedef struct Info { char pad[0x29A4]; float x29A4; char pad2[12]; int x29B4, x29B8; } Info;
typedef struct Other { char pad[0x58]; float x58; } Other;
typedef struct Obj { int x0; Other *x4; Info *x8; char pad[0x74]; Value x80, x90; char pad2[0x160]; int x200, x204; unsigned char x208; } Obj;
extern float func_00203C28(Value *);
extern void func_00203DC8(Value *, int);
extern void func_00203E10(Value *, float);
extern void func_00202E38(Value *, int, int, float);
extern int func_00222B08(int, int);
extern int func_00238F60(void);
void func_00172118(Obj *obj) {
 Value a; float amount;
 if (obj->x208) {
  a = obj->x90;
  if (func_00203C28(&a) >= 1.0f) {
   obj->x208=0;
   obj->x204=func_00222B08(obj->x8->x29B4,obj->x8->x29B8);
   obj->x200=0;
   func_00202E38(&obj->x80,1,1,150.0f);
  }
 } else {
  obj->x200 += func_00238F60();
  if (obj->x200 > obj->x204 && obj->x4->x58 > 0.05f) {
   amount=obj->x8->x29A4;
   obj->x208=1;
   obj->x200=0;
   a=obj->x90; func_00203DC8(&a,0);
   a=obj->x90; func_00203E10(&a,amount);
   func_00202E38(&obj->x90,1,1,150.0f);
  }
 }
}
