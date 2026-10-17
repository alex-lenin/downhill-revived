/* UNIT 0x0019AB60 0x0019AC38 */
typedef struct Obj { char pad[0x504]; int x504,x508; } Obj;
extern int D_003F52D0[]; extern char *D_0026F998[];
extern void func_0023D650(int,int*,int*);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,char*);
void func_0019AB60(Obj *obj) { int width,height,half; if(obj->x508>0 && obj->x504) {func_0023D650(D_003F52D0[8],&width,&height); half=width/2; func_0020F518(0,160,100,50,128); func_0020FB80(0,288,224,-107,-32); func_0020F9F0(0,D_003F52D0[8],half,184,D_0026F998[obj->x504]); obj->x508--; } }
