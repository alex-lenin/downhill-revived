/* UNIT 0x00159CE0 0x0015A4F0 .rodata=0x28A2E0:0x28A310 .sdata=0x29C6E0:0x29C6E4 */
struct Handle { int x0, x4; short x8, xA; int xC; };
struct Actor { char pad0[0x28C4]; float x28C4,x28C8; };
struct Obj {
 int x0,state; unsigned int **x8; void *xC; char pad10[0x10]; Actor *x20; int x24;
 Handle x28,x38,x48,x58,x68,x78,x88,x98;
 char padA8[0x30]; Handle xD8; char padE8[0x98]; void *x180;
 char pad184[0x3C]; float x1C0,x1C4,x1C8,x1CC,x1D0;
 char pad1D4[0xC]; short x1E0,x1E2,x1E4,x1E6,x1E8,x1EA;
};
float D_0029C6E0=2.0f;
extern int D_003041B0[];
extern void func_00203CB8(Handle *);
extern float func_00203C28(Handle *);
extern int func_00204230(Handle *);
extern void func_00203DC8(Handle *,int);
extern void func_00203E10(Handle *,float);
extern void func_00204450(Handle *,int);
extern void func_00202E38(Handle *,int,int,float);
extern float func_00222B68(float,float);
extern int func_00122258(void *,unsigned int);
extern int func_00159468(Actor *,int);
extern void func_002182A8(void *,int);
extern void func_0022DDD8(void *);
void func_00159CE0(Obj *obj,int state) {
 Handle a,b,c;
 if(obj->state!=state) {
  if(obj->x28.x0) (a=obj->x28,func_00203CB8(&a));
  if(obj->x38.x0) (a=obj->x38,func_00203CB8(&a));
  if(obj->x48.x0) (a=obj->x48,func_00203CB8(&a));
  if(obj->x58.x0) (a=obj->x58,func_00203CB8(&a));
  if(obj->x78.x0) (a=obj->x78,func_00203CB8(&a));
  if(obj->x88.x0) (a=obj->x88,func_00203CB8(&a));
 }
 obj->state=state; obj->x1E0=0; obj->x1E6=0;
 switch(state) {
 case 1: {
 int flag=0;
 if((a=obj->x58,func_00203C28(&a))>=1.0f || !(b=obj->x58,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x58,func_00203DC8(&c,0));
 (c=obj->x58,func_00203E10(&c,obj->x1C8));
 func_00202E38(&obj->x58,1,1,150.0f);
 }
 break; }
 case 2: {
 int flag=0;
 if((a=obj->x68,func_00203C28(&a))>=1.0f || !(b=obj->x68,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x68,func_00203DC8(&c,1));
 (c=obj->x68,func_00203E10(&c,obj->x1CC));
 func_00202E38(&obj->x68,1,1,150.0f);
 }
 obj->x1EA=(short)func_00222B68(obj->x20->x28C4*60.0f,obj->x20->x28C8*60.0f);
 break; }
 case 3: {
 int flag=0;
 if((a=obj->x78,func_00203C28(&a))>=1.0f || !(b=obj->x78,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x78,func_00203DC8(&c,0));
 (c=obj->x78,func_00203E10(&c,obj->x1D0));
 func_00202E38(&obj->x78,1,1,150.0f);
 }
 break; }
 case 4: {
 int flag=0;
 if((a=obj->x28,func_00203C28(&a))>=1.0f || !(b=obj->x28,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x28,func_00203DC8(&c,0));
 (c=obj->x28,func_00203E10(&c,obj->x1C0));
 func_00202E38(&obj->x28,1,1,150.0f);
 }
 break; }
 case 5: {
 int flag=0;
 if((a=obj->x38,func_00203C28(&a))>=1.0f || !(b=obj->x38,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x38,func_00203DC8(&c,1));
 (c=obj->x38,func_00203E10(&c,obj->x1C4));
 func_00202E38(&obj->x38,1,1,150.0f);
 }
 break; }
 case 6: { short value=obj->x1E8; obj->x1E8=value>0?value:1; break; }
 case 7: {
 int flag=0;
 if((a=obj->x98,func_00203C28(&a))>=1.0f || !(b=obj->x98,func_00204230(&b))) flag=1;
 if((unsigned char)flag) {
 (c=obj->x98,func_00203DC8(&c,0));
 (c=obj->x98,func_00204450(&c,10));
 (c=obj->x98,func_00203E10(&c,D_0029C6E0));
 func_00202E38(&obj->x98,1,1,150.0f);
 }
 break; }
 case 10: {
 int value=func_00159468(obj->x20,func_00122258(D_003041B0,**obj->x8 >> 18));
 if(value) {
 Handle *h=&obj->xD8;
 h->x0=(int)D_003041B0; h->x4=value; h->xA=-1; h->x8=-1; obj->state=0;
 } else { func_002182A8(obj->x8,0); func_002182A8(obj->xC,0); }
 func_0022DDD8(obj->x180); obj->x180=0;
 break; }
 case 0: case 8: case 9: default: break;
 }
}
