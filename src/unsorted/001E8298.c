/* UNIT 0x001E8298 0x001E8440 .rodata=0x2949C0:0x2949E0 */
struct VTable { char pad[0x20]; short delta; short pad22; int (*call)(void *); };
struct Rider { char p0[0x78D4]; int x78D4; char p78D8[0xA0]; int x7978; };
struct Obj { struct Rider *rider; char p4[0x24]; int x28; int x2C; char p30[0x19C]; int x1CC; char p1D0[0x10]; float x1E0,x1E4; char p1E8[9]; char x1F1; char p1F2[2]; char x1F4; char p1F5; char x1F6; };
struct Entry { char p0[0x34]; float x34,x38; char p3C[0x18]; };
extern struct Entry D_00272888[];
extern void func_0012FE38(void *,void *),func_001AC578(void *),func_001ACAE0(void *,float);
extern int func_002205A0(int),func_00220618(int,int),func_001E5CC8(void *,int);
extern void func_001E8440(struct Obj *),func_001E86E8(struct Obj *),func_001E8B70(struct Obj *),func_001E9250(struct Obj *),func_001E95B8(struct Obj *);
void func_001E8298(struct Obj *o) {
 struct Rider *r=o->rider; void *s=(char *)r+0x62E0; void *t=(char *)r+0x1E40; struct VTable *v; struct Entry *e;
 o->x1F6=0; v=*(struct VTable **)((char *)s+4);
 if(v->call((char *)s+v->delta)) { func_0012FE38(r,s); return; }
 if(r->x78D4 || !func_00220618(0x1000,func_002205A0(r->x7978))) o->x1F1=1;
 func_001AC578(t);
 if(!r->x78D4 && !func_001E5CC8((char *)r+0x6610,o->x28)) o->x1F4=1;
 switch(o->x1CC) { case 1:func_001E8440(o);break;case 2:func_001E86E8(o);break;case 3:func_001E8B70(o);break;case 5:func_001E9250(o);break;case 4:func_001E95B8(o);break; }
 e=&D_00272888[o->x2C];
 if(e->x34!=0.0f) func_001ACAE0((char *)o+0x104,o->x1E0);
 if(e->x38!=0.0f) func_001ACAE0((char *)o+0x168,o->x1E4);
}
