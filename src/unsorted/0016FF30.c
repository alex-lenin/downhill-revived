/* UNIT 0x0016FF30 0x00170160 */
struct Vec { float x,y,z,w; };
struct Part { char pad0[0x10]; struct Vec pos; char pad20[0x20]; char matrix[0x60]; struct Vec angle; };
struct Obj { char pad0[0x1F0]; struct Part *x1F0; char pad1F4[0x40]; unsigned char x234; };
extern char D_002AF2C0[];
extern int func_0011C0D0(void *,void *,int,float);
extern void func_001DD030(void *,void *,int,int);
extern void func_0020B880(void *,void *,float);
extern void func_00222460(void *,void *);
extern void func_00221F50(void *,void *);
extern float func_00235EA8(float,float,float,float);
extern float fabsf(float);
static inline void Wrap(struct Part *part,float target,int axis) {
 float current=((float *)&part->angle)[axis]; float delta=target-current;
 float adjusted;
 if (delta < 0.0f) adjusted = delta;
 __asm__ volatile ("1:");
 if (fabsf(delta)>3.1415927f) {
  if(current<target) adjusted=current+6.2831855f;
  else adjusted=current-6.2831855f;
  ((float *)&part->angle)[axis]=adjusted;
 }
}
void func_0016FF30(struct Obj *obj,int value,int force) {
 struct Vec angle; struct Part *p; float current;
 if(force) {
  func_001DD030((char *)obj+0x10,obj->x1F0->matrix,value,0);
  p=obj->x1F0; func_0020B880(&p->pos,p->matrix,30.0f);
  p=obj->x1F0; func_00222460(p->matrix,&p->angle);
  return;
 }
 if(obj->x234 || func_0011C0D0(D_002AF2C0,&obj->x1F0->pos,1,150.0f)) {
  func_001DD030((char *)obj+0x10,obj->x1F0->matrix,value,0);
  func_00222460(obj->x1F0->matrix,&angle);
  obj->x1F0->angle.z=angle.z;
  Wrap(obj->x1F0,angle.x,0);
  obj->x1F0->angle.x=func_00235EA8(obj->x1F0->angle.x,angle.x,0.04f,0.001f);
  Wrap(obj->x1F0,angle.y,1);
  obj->x1F0->angle.y=func_00235EA8(obj->x1F0->angle.y,angle.y,0.04f,0.001f);
  p=obj->x1F0; func_00221F50(p->matrix,&p->angle);
 }
}
