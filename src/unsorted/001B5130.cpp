/* UNIT 0x001B5130 0x001B5368 */
#include "render_wait.h"
class Target { public: char pad[0x4F8]; virtual void m1(); virtual void m2(); virtual void m3(); virtual void m4(int,int,int); };
class Obj { public:
 char pad0[4]; unsigned char x4,x5,x6,x7,x8; char pad9[0x17]; int x20; char pad24[0x24]; char *x48; char pad4C[0x14]; unsigned char x60;
 void func_001B4DD0(); void func_001B5130();
};
struct Small { char pad[4]; unsigned char x4; };
struct State { char pad[0x134]; Small *x134; char pad138[0x18]; Target *x150; };
extern State D_003F52D0;
extern int D_0029E10C,D_0029E110;
extern char D_004B1F40[],D_00513B00[];

extern void func_001B53C0(Obj *);
extern void func_0020F2A8(int);
extern void func_00210460(void *,int,int);
extern void func_002161B8(int,int,int,int);
extern void func_00218720(Small *,int,int,int);
extern int func_00228328(int);
extern void func_0022DE40();
extern void func_00238B40();
extern void func_00238C00(int *);
extern void func_00238D98(int *);
extern void func_0023D618(int,int *,int *);
extern int func_002433A8(int,int);
extern void func_002448B8(void *,short,short,short);
extern void func_00244B20(void *,short,short,short);
void Obj::func_001B5130() {
 int x,y;
 char *p;
 if(!x60) this->func_001B4DD0();
 x60=0;
 if(!x4) {
 func_0020F2A8(x20);
 WaitForRenderLegacy(D_004B1F40);
 if(!x5) {
 func_002161B8(x20,1,(*(unsigned char *)&D_0029E10C ^ 1)&1,D_0029E110);
 func_002433A8(0,0);
 }
 func_00210460(D_00513B00,x20,1);
 func_002433A8(0,0);
 func_00228328(x20);
 if(x7) func_0022DE40();
 func_002433A8(0,0);
 if(D_003F52D0.x134) { if(D_003F52D0.x134->x4) func_00218720(D_003F52D0.x134,(*(unsigned char *)&D_0029E10C ^ 1)&1,D_0029E110,x20); }
 }
 if(D_003F52D0.x150) D_003F52D0.x150->m4(D_0029E10C,D_0029E110,x20);
 func_002433A8(0,0);
 if(x4 && !x8) func_001B53C0(this);
 func_00238B40();
 if(x4) {
 func_0023D618(0,&x,&y);
 if(D_0029E10C & 1) p=x48+0x200; else p=x48+0x90;
 func_002448B8(p,*(short *)&x, (short)*(unsigned short *)&y, *(short *)&D_0029E110);
 if(D_0029E10C & 1) p=x48+0x280; else p=x48+0x110;
 func_00244B20(p,*(short *)&x, (short)*(unsigned short *)&y, *(short *)&D_0029E110);
 }
 D_0029E10C++;
 func_00238C00(&D_0029E110);
 func_00238D98(&D_0029E110);
}
