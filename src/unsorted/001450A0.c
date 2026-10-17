/* UNIT 0x001450A0 0x00145368 */
typedef struct Handle { int x0,x4,x8,xC; } Handle;
typedef struct Obj { int x0,x4; char pad8[8]; Handle x10,x20,x30; char pad40[0x168]; int x1A8,x1AC,x1B0; } Obj;
extern void func_00203CB8(Handle *);
extern void func_00203DC8(Handle *,int);
extern void func_00202E38(Handle *,int,int,float);
extern int func_00222BD8(int,int);
void func_001450A0(Obj *obj,int state) {
 Handle h; int duration;
 Handle *a=&obj->x10,*b,*c;
 obj->x1A8=0; obj->x1B0=0;
 if(obj->x10.x0) {h=obj->x10;func_00203CB8(&h);}
 b=&obj->x20;
 if(obj->x20.x0) {h=obj->x20;func_00203CB8(&h);}
 c=&obj->x30;
 if(obj->x30.x0) {h=obj->x30;func_00203CB8(&h);}
 switch(state) {
 case 2:
  if(obj->x10.x0) {h=obj->x10;func_00203DC8(&h,1);func_00202E38(a,1,1,450.0f);}
  obj->x1B0=func_00222BD8(0,180);break;
 case 3:
  if(obj->x20.x0) {h=obj->x20;func_00203DC8(&h,1);func_00202E38(b,1,1,450.0f);}
  obj->x1B0=func_00222BD8(0,420);break;
 case 4:
  if(obj->x30.x0) {h=obj->x30;func_00203DC8(&h,1);func_00202E38(c,1,1,450.0f);}
  obj->x1B0=func_00222BD8(0,300);break;
 default:
  if(obj->x10.x0) {h=obj->x10;func_00203CB8(&h);}
  if(obj->x20.x0) {h=obj->x20;func_00203CB8(&h);}
  if(obj->x30.x0) {h=obj->x30;func_00203CB8(&h);}
  break;
 }
 obj->x4=state;
}
