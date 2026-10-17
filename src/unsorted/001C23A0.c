/* UNIT 0x001C23A0 0x001C2458 */
struct Obj { char pad[12]; float xC,x10,x14,x18,x1C; int x20; float x24; };
extern int func_00238F48(void);
extern void func_001C22D0(struct Obj *);
float func_001C23A0(struct Obj *p) {
 float x,limit; int time=func_00238F48();
 if(p->x14*60.0f < (float)(time-p->x20)) func_001C22D0(p);
 limit=p->xC; x=p->x1C+p->x24; p->x1C=x;
 if(limit<x) { p->x1C=limit; func_001C22D0(p); }
 { float lower=-p->xC;
 if(p->x1C<lower) { p->x1C=lower; func_001C22D0(p); } }
 return p->x1C;
}
