/* UNIT 0x001A3C18 0x001A3E08 */
struct Menu { char pad[0x20]; int x20; };
extern struct Menu D_003F52D0;
extern int D_004B26AC[];
extern int *D_0029DE68;
extern char D_0028E7C0[];
extern void func_0023D650(int,int*,int*);
extern void func_0020F648(int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,void*);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,void*);
void func_001A3C18(int unused,int alpha) {
 int width,height;
 func_0023D650(D_003F52D0.x20,&width,&height);
 width/=2;
 func_0020F648(0,36,24);
 func_0020F518(0,200,200,0,alpha);
 if(D_004B26AC[0]==13) func_0020F9F0(0,D_003F52D0.x20,width,5,D_0028E7C0);
 else func_0020F9F0(0,D_003F52D0.x20,width,5,(void*)D_0029DE68[0x234/4]);
 func_0020FB80(0,256,192,-107,-32);
 func_0020F518(0,110,110,200,alpha);
 func_0020F9A8(0,D_003F52D0.x20,170,30,(void*)D_0029DE68[0x220/4]);
 func_0020F9A8(0,D_003F52D0.x20,270,30,(void*)D_0029DE68[0x228/4]);
 func_0020F9A8(0,D_003F52D0.x20,380,30,(void*)D_0029DE68[0x230/4]);
 func_0020F9A8(0,D_003F52D0.x20,70,38,(void*)D_0029DE68[0xc8/4]);
 func_0020F9A8(0,D_003F52D0.x20,164,38,(void*)D_0029DE68[0x224/4]);
 func_0020F9A8(0,D_003F52D0.x20,270,38,(void*)D_0029DE68[0x22c/4]);
 func_0020F9A8(0,D_003F52D0.x20,370,38,(void*)D_0029DE68[0x220/4]);
}
