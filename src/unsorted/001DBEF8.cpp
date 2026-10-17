/* UNIT 0x001DBEF8 0x001DC0E8 */
class Action { public: int x0; virtual int v0(); virtual int v1(); virtual int v2(); virtual int active(); };
struct Rider { char pad[0x7a69]; unsigned char x7a69; };
struct Obj { Rider *x0; int x4,x8; float xc,x10,x14,x18; };
extern "C" void func_001AC578(void *),func_0012FE38(Rider *,Action *),func_001DC238(Obj *),func_00162520(Rider *);
extern "C" float func_00236088(float,float,float,float);
void func_001DBEF8(Obj *o) {
 Rider *r=o->x0; Action *a;
 func_001AC578((char *)r+0x1e40);
 if((a=(Action *)((char *)r+0x5e40))->active() || (a=(Action *)((char *)r+0x5e70))->active() || (a=(Action *)((char *)r+0x5e90))->active() || (a=(Action *)((char *)r+0x5ff0))->active() || (a=(Action *)((char *)r+0x6210))->active()) func_0012FE38(r,a);
 else if(!r->x7a69) func_0012FE38(r,(Action *)((char *)r+0x4ed0));
 else if((a=(Action *)((char *)r+0x62e0))->active()) func_0012FE38(r,a);
 else if((a=(Action *)((char *)r+0x6310))->active()) func_0012FE38(r,a);
 o->xc=func_00236088(o->xc,1.0f,0.1f,0.05f);
 o->x10=func_00236088(o->x10,1.0f,0.1f,0.05f);
 o->x14=func_00236088(o->x14,o->x18,0.03f,0.005f);
 func_001DC238(o);func_00162520(r);
}
