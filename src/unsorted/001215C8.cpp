/* UNIT 0x001215C8 0x001217E8 */
class Base { public: virtual void v0(); virtual void v1(); virtual void v2(); virtual void *v3(); };
struct Flag { int x0; unsigned char x4; };
class Obj: public Base { public: char pad4[12]; unsigned int **x10; Flag *x14; char pad18[0x1c]; void *x34; char pad38[0x74]; char xAC[16], xBC[16], xCC[16]; unsigned int GetType() { return **x10 & 0xFFFC0000; } };
struct Vec { float x,y,z,w; Vec(float a,float b,float c,float d) {x=a;y=b;z=c;w=d;} };
struct Params { void *x0,*x4,*x8,*xC; int rest[6]; };
extern "C" {
extern char D_0038DCE0[];
void func_001BE158(void *, void *);
struct Receiver { void func_001BE158(void *); };
struct Manager { char pad[0x1230]; Receiver x1230; Receiver *Get() { return &x1230; } };
extern Manager D_00398C40;
void *func_00164A10(void *);
void func_0011E568(Obj *, int);
void *func_00254204(void *, int, unsigned int);
void func_00164EE8(void *, int, void *, float *, Params *);
void func_00165408(void *, int, int);
void func_001BE158(void *, void *);
void func_001215C8(Obj *obj, int arg1, int arg2) {
 Params p;

 unsigned int type=**obj->x10 & 0xFFFC0000;
 if ((type==0x12E00000 || type==0x134C0000 || type==0x132C0000 || type==0x138C0000 || type==0x13940000 || type==0x17440000 || type==0x174C0000 || type==0x17340000 || type==0x173C0000) && !obj->x34) {
  obj->x34=func_00164A10(D_0038DCE0);
  if(obj->x34) {
   func_0011E568(obj,7);
   func_00254204(&p,0,40);
   p.x0=obj->xAC; p.x4=obj->xBC; p.xC=obj->xCC;
   Vec vec(4.0f,4.0f,0.0f,0.0f);
   int type2=**obj->x10 & 0xFFFC0000;
   if(type2==0x12E00000) func_00164EE8(obj->x34,1,obj->v3(),(float *)&vec,&p);
   else if(type2==0x134C0000 || type2==0x17440000 || type2==0x174C0000) func_00164EE8(obj->x34,5,obj->v3(),(float *)&vec,&p);
   else func_00164EE8(obj->x34,6,obj->v3(),(float *)&vec,&p);
   func_00165408(obj->x34,arg2,arg1);
   if(obj->x14) obj->x14->x4=0;
   D_00398C40.Get()->func_001BE158(obj->v3());
  }
 }
}
}
