/* UNIT 0x00145920 0x00145998 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char pad[0x30]; Vec x30; } Obj;
extern void func_001457A8(Obj *);
extern float func_00203C28(Vec *);
extern void func_001450A0(Obj *,int);
void func_00145920(Obj *p) {
 Vec v;
 func_001457A8(p);
 v=p->x30;
 if(func_00203C28(&v)>0.95f) func_001450A0(p,3);
}
