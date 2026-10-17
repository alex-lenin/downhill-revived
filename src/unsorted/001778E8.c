/* UNIT 0x001778E8 0x00177A98 */
typedef struct Part { char pad[0xB8]; float xB8; } Part;
typedef struct Obj { char p0[0x1178]; Part *x1178; char p1[0x1280-0x117C]; int x1280; char p2[0x1361-0x1284]; unsigned char x1361,x1362; char p3[9]; int x136C,x1370; } Obj;
extern int D_004B26AC[];
extern int D_0026F898[];
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F588(int,int,int,int,int,int);
extern void func_0017B5C8(Obj *,int,float,float);
extern int func_0020F860(int,int);
static inline int greater(int a,int b) { return a>b; }
void func_001778E8(Obj *o) {
 int alpha=180;
 if(o->x136C<0 || D_004B26AC[0]==14) { o->x1370=0; return; }
 if(o->x1370>240) {
  alpha=(300-o->x1370)*3;
  if(!greater(181,alpha)) alpha=180; if(!greater(alpha,0)) alpha=0;
  if(o->x1178) o->x1178->xB8=1.0f-(float)alpha*0.0055555557f;
  goto fading;
 } else if(o->x1370<60) {
  alpha-= (60-o->x1370)*3;
  if(o->x1178) o->x1178->xB8=1.0f-(float)alpha*0.0055555557f;
  fading:
  o->x1361=1; o->x1362=1;
 } else { o->x1361=0; o->x1362=1; }
 func_0020FB80(0,224,208,-91,-48);
 func_0020F588(o->x1280,0,110,110,200,alpha);
 func_0017B5C8(o,D_0026F898[o->x136C],0.05f,1.6f);
 func_0020F860(0,D_0026F898[o->x136C]);
 o->x1370 = o->x1370 - (int)1;
}
