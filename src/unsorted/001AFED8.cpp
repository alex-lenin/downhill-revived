/* UNIT 0x001AFED8 0x001B00B8 */
struct Desc { int a,b,c; float d; } __attribute__((packed));
struct Obj { char pad[0x44]; float x44; char pad48[0x1c]; struct Desc x64; float x74; char pad78[0x64]; struct Desc xDC; float xEC; char *xF0; };
extern "C" int func_00204230(struct Desc*);
extern "C" void func_00203DC8(struct Desc*,int);
extern "C" void func_00202E38(struct Desc*,int,int,float);
extern "C" float func_001385A8(void*);
extern "C" void func_001ACB70(struct Obj*,float);
extern "C" void func_00203E10(struct Desc*,float);
extern "C" void func_001AFED8(struct Obj *obj) {
 struct Desc check,desc; float value;
 if(obj->xF0) {
  check=obj->x64;
  if(((func_00204230(&check)^1)&255)) {
   desc=obj->x64;
   func_00203DC8(&desc,1);
   func_00202E38(&obj->x64,1,1,0.0f);
  } else {
   value=obj->x74*func_001385A8(obj->xF0+0x40c0);
   value = (value >? 0.0f) <? 1.0f;
   func_001ACB70(obj,value); obj->x44=value;
  }
  check=obj->xDC;
  if(((func_00204230(&check)^1)&255)) {
   desc=obj->xDC; func_00203DC8(&desc,1);
   func_00202E38(&obj->xDC,1,1,0.0f);
  } else {
   float other=obj->xEC*func_001385A8(obj->xF0+0x40c0);
   desc=obj->xDC;
   other = (other >? 0.0f) <? 1.0f;
   func_00203E10(&desc,other);
  }
 }
}
