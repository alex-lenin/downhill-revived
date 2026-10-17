/* UNIT 0x001DC108 0x001DC238 */
typedef struct Obj Obj;
typedef struct Owner { char pad[0x4ecc]; Obj *x4ecc; } Owner;
struct Obj { Owner *x0; char pad[8]; float xc,x10,x14,x18,x1c; };
extern float func_00236088(float,float,float,float);
extern void func_001DC238(Obj *);
int func_001DC108(Obj *obj) {
 int result;
 if(obj->x0->x4ecc == obj) return 0;
 {
  obj->x10=func_00236088(obj->x10,0.0f,0.2f,0.1f);
  obj->xc=func_00236088(obj->xc,0.0f,0.2f,0.1f);
  obj->x1c=func_00236088(obj->x1c,0.0f,0.2f,0.1f);
  if(obj->x1c==0.0f) obj->x14=func_00236088(obj->x14,0.0f,0.035f,0.015f);
  func_001DC238(obj);
  result=0;
  if(obj->xc!=0.0f || obj->x10!=0.0f || obj->x14!=0.0f) result=1;
 }
 return result;
}
