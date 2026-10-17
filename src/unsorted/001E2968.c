/* UNIT 0x001E2968 0x001E2A40 */
typedef struct { char pad[0x514];int x514,x518; } Obj;
typedef struct { char pad[0x20];int x20; } Global;
extern Global D_003F52D0;
extern int D_00271820[];
extern void func_0023D650(int,int *,int *);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,int);
void func_001E2968(Obj *obj) {
 int dims[2];int half;
 if(obj->x518>0 && obj->x514) {
 func_0023D650(D_003F52D0.x20,&dims[0],&dims[1]);
 half=dims[0]/2;
 func_0020F518(0,160,100,50,128);
 func_0020FB80(0,288,224,-107,-32);
 func_0020F9F0(0,D_003F52D0.x20,half,184,D_00271820[obj->x514]);
 obj->x518--;
 }
}
