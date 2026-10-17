/* UNIT 0x0015AE10 0x0015AEE8 */
typedef struct Sound { int x0,x4,x8,xC; } Sound;
typedef struct Settings { char pad[0x28B4]; float x28B4,x28B8,x28BC; } Settings;
typedef struct Obj { char pad[0x20]; Settings *x20; char pad24[0x24]; Sound x48; char pad58[0x158]; float x1B0; int x1B4; float x1B8,x1BC; char pad1C0[0x1C]; int x1DC; char pad1E0[2]; char x1E2; char pad1E3; short x1E4; } Obj;
extern void func_00203DC8(Sound *,int);
extern void func_00203E10(Sound *,float);
extern void func_00202E38(Sound *,int,int,float);
extern void func_0015BEA0(Obj *,float);
void func_0015AE10(Obj *obj) {
 Sound sound;
 obj->x1B0=obj->x20->x28B4;
 obj->x1B8=obj->x20->x28B8;
 obj->x1BC=obj->x20->x28BC;
 sound=obj->x48; func_00203DC8(&sound,1);
 sound=obj->x48; func_00203E10(&sound,obj->x1B8);
 func_00202E38(&obj->x48,1,1,150.0f);
 obj->x1E4=0; obj->x1E2=1; obj->x1DC=0;
 func_0015BEA0(obj,obj->x1B0);
}
