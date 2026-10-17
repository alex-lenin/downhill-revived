/* UNIT 0x00177770 0x001778D0 */
typedef struct State {char pad[0x6390];int x6390;} State;
typedef struct Obj {char pad[0x1164];State *x1164;char pad1168[0x118];int x1280;} Obj;
extern char D_0029C9A0[],D_0029C9A8[],D_0029C9B0[],D_0029C9B8[],D_0029C9C0[];
extern void func_0017B520(Obj *,void *,float,float);
extern void func_0020F588(int,int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
void func_00177770(Obj *obj) {
 int type=obj->x1164->x6390;
 func_0020F588(obj->x1280,0,110,110,200,170);
 func_0020FB80(0,224,192,-107,-32);
 if(type==0) func_0017B520(obj,D_0029C9A0,1.84f,1.0f);
 else if(type==1) func_0017B520(obj,D_0029C9A8,1.84f,1.0f);
 else if(type==2) func_0017B520(obj,D_0029C9B0,1.84f,1.0f);
 else if(type==3) func_0017B520(obj,D_0029C9B8,1.84f,1.0f);
 else if(type==4) func_0017B520(obj,D_0029C9C0,1.84f,1.0f);
}
