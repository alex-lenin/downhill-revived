/* UNIT 0x0011F1E0 0x0011F298 */
typedef struct Obj { char pad[8]; int x8; char padC[0x250-12]; float x250; } Obj;
extern void func_0011E568(Obj *,int);
extern int func_00222BD8(int,int);
void func_0011F1E0(Obj *obj) {
 float distance=obj->x250;
 int state;
 if(distance<6400.0f) func_0011E568(obj,5);
 else {
  state=obj->x8;
  if(state==5) {
   if(distance>32400.0f && func_00222BD8(0,100)==42) func_0011E568(obj,2);
  } else if((unsigned)(state-2)>=3) func_0011E568(obj,4);
 }
}
