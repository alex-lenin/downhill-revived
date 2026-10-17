/* UNIT 0x00135100 0x00135178 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char pad[0x240]; Vec x240,x250,x260,x270,x280; char gap[0x10]; char x2A0[1]; } Obj;
extern void func_00221F20(void *);
static __inline__ void set(Vec *v,float x,float y,float z) { v->x=x; v->y=y; v->z=z; }
void func_00135100(Obj *p) {
 set(&p->x240,0,0,0);
 set(&p->x250,0,0,1);
 set(&p->x260,0,0,1);
 set(&p->x270,0,0,1);
 set(&p->x280,0,0,1);
 func_00221F20(p->x2A0);
}
