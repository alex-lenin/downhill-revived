/* UNIT 0x001AD580 0x001AD700 */
typedef struct { float x,y,z,w; } Vec;
typedef struct {
 char pad0[0x18]; char x18; char pad19[3];
 float x1C,x20,x24,x28,x2C,x30;
 char pad34[0x24]; Vec x58;
} Obj;
extern float func_002041D0(Vec *);
void func_001AD580(Obj *obj,float target,float duration) {
 Vec vector;
 float current,delta,result;
 current=obj->x24;
 obj->x30=current;
 while(current>=1.0f) current-=1.0f;
 while(target>=1.0f) target-=1.0f;
 delta=target-current;
 if(delta>0.5f) delta-=1.0f;
 else if(delta< -0.5f) delta+=1.0f;
 obj->x18=1;
 vector=obj->x58;
 result=func_002041D0(&vector)*0.016666668f;
 obj->x2C=target;
 result=delta*((result/duration)*3.0f);
 obj->x20=result;
 obj->x1C=result;
}
