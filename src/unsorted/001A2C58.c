/* UNIT 0x001A2C58 0x001A2E28 */
typedef struct { char pad[0x20]; int x20; } State;
typedef struct { char pad[0xBC]; int xBC,xC0,xC4,xC8,xCC,xD0,xD4,xD8,xDC,xE0; } Text;
extern State D_003F52D0;
extern Text *D_0029DE68;
extern void func_0023D650(int,int *,int *);
extern void func_0020F648(int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,int);
void func_001A2C58(int unused,int alpha) {
 int dimensions[2];
 func_0023D650(D_003F52D0.x20,&dimensions[0],&dimensions[1]);
 dimensions[0]/=2;
 func_0020F648(0,36,24);
 func_0020F518(0,200,200,0,alpha);
 func_0020F9F0(0,D_003F52D0.x20,dimensions[0],5,D_0029DE68->xBC);
 func_0020FB80(0,256,192,-91,-32);
 func_0020F518(0,110,110,200,alpha);
 func_0020F9A8(0,D_003F52D0.x20,15,32,D_0029DE68->xC4);
 func_0020F9A8(0,D_003F52D0.x20,82,32,D_0029DE68->xC8);
 func_0020F9A8(0,D_003F52D0.x20,166,32,D_0029DE68->xCC);
 func_0020F9A8(0,D_003F52D0.x20,256,24,D_0029DE68->xD0);
 func_0020F9A8(0,D_003F52D0.x20,244,32,D_0029DE68->xD4);
 func_0020F9A8(0,D_003F52D0.x20,340,24,D_0029DE68->xD8);
 func_0020F9A8(0,D_003F52D0.x20,320,32,D_0029DE68->xDC);
 func_0020F9A8(0,D_003F52D0.x20,414,32,D_0029DE68->xE0);
}
