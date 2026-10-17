/* UNIT 0x001238B8 0x00123AD0 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
struct Model { char pad[0x792C]; char *x792C; };
struct Vtable { char pad[0x20]; short delta; short pad2; char *(*call)(void *); };
struct Virtual { struct Vtable *vt; };
struct Obj { char pad[0x68]; char x68[0x68]; float xD0; char padD4[0x5C]; struct Virtual *x130; int x134; struct Model *x138; int x13C,x140; struct Model *x144; };
extern float func_002223F0(void *,void *);
extern float func_00126D30(struct Obj *,void *,void *);
extern float func_00222640(void *);
extern void func_002087C0(float *);
extern float func_00236130(float,float,float,float);
extern float func_00235EA8(float,float,float,float);
extern float func_001ACBA0(void *);
extern void func_001ACAE0(void *,float);
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
void func_001238B8(struct Obj *obj) {
 Vec pos; float angle,heading,result,value; char *entity; void *point;
 struct Model *target;
 entity=obj->x130->vt->call((char *)obj->x130+obj->x130->vt->delta);
 target=obj->x144; if(!target) target=obj->x138; if(target) { COPY_VECTOR(pos,target->x792C+0x10); }

 if((obj->x144 || obj->x138) && func_002223F0(point=entity+0x10,&pos)<200.0f) {
  heading=func_00126D30(obj,point,&pos);
  angle=heading-func_00222640(entity+0x50);
  func_002087C0(&angle);
  value=func_00236130(obj->xD0,angle,0.05f,0.0001f);
  obj->xD0=value;
  if(value>2.3561945f || value< -2.3561945f) { point=obj->x68; result=func_00235EA8(func_001ACBA0(point),0.5f,0.05f,0.0001f); }
  else {point=obj->x68; result=func_00235EA8(func_001ACBA0(point),obj->xD0*0.21220659f+0.5f,0.05f,0.0001f);}
 } else { point=obj->x68; result=func_00235EA8(func_001ACBA0(point),0.5f,0.05f,0.0001f); obj->xD0=0.0f; }
 func_001ACAE0(point,result);
}
