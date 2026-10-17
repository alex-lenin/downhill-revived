/* UNIT 0x0013BA60 0x0013C128 .sdata=0x29C470:0x29C4C0 .sbss=0x29DD10:0x29DD20 */
#include "ui_style.h"
#include "animation.h"
typedef unsigned int u128 __attribute__((mode(TI)));
struct Base {
 int x0; char pad4[0x458]; int x45C, x460, x464, x468, x46C, x470, x474, x478;
 char pad47C[0x2C]; int x4A8; char pad4AC[0x4C];
 virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
 virtual void v6(int); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void *v14(int);
};
struct Obj: Base { int x4FC; Obj *x500; char x504[0x1C]; int x520, x524, x528; };
struct Manager { char pad[0x64]; int x64; char pad68[0xC5]; unsigned char x12D; char pad12E[0x22]; Obj *x150; };
struct Global { char pad[0x10C]; int x10C; char pad110[0x180]; int x290; };
struct Text { char pad[0x44]; char *x44, *x48; };
unsigned char D_0029C470=0, D_0029C471=0;
char D_0029C478[8] __attribute__((aligned(8)))="Ranim";
char D_0029C480[8] __attribute__((aligned(8)))="Banim";
float D_0029C488[2]={140.0f,0.0f};
char D_0029C490[8] __attribute__((aligned(8)))="SHELL";
char D_0029C498[8] __attribute__((aligned(8)))="NGP";
char D_0029C4A0[8] __attribute__((aligned(8)))="RTX";
char D_0029C4A8[8] __attribute__((aligned(8)))="TEX";
char D_0029C4B0[4] __attribute__((aligned(8)))="PTR";
int D_0029C4B4=0;
int D_0029C4B8=130, D_0029C4BC=31;
int D_0029DD10 __attribute__((section(".sbss")))=0;
int D_0029DD14 __attribute__((section(".sbss")))=0;
int D_0029DD18 __attribute__((section(".sbss")))=0;
int D_0029DD1C __attribute__((section(".sbss")))=0;

extern char D_0029C4C0[], D_0029C4C8[];
extern Text *D_0029DD30;
extern Manager D_003F52D0;
extern Global D_004B25A0;
extern char D_00361690[],D_003616B0[],D_003623A0[],D_00362468[],D_00362990[],D_00362E90[],D_0036B730[],D_004B2A90[],D_004C7A30[];
extern void func_0013B9C0();
extern void func_0013C838(Obj *,int);
extern void func_00190358(void *);
extern int func_00190D60(void *,int,int);
extern void func_00190660(void *,void *);
extern void func_001C57A8(void *,u128);
extern void func_0018DBA8(void *);
extern void func_00208500(void *);
extern void func_0013A170();
extern void func_00140C38(void *);
extern void *func_0013AF60(int);
extern void func_00218498(void *,int);
extern void func_001F57D0(int,int);
extern void func_0013C338(Obj *),func_0013C5E0(Obj *),func_0013C6A8(Obj *);
extern int func_0018D8B8(void *);
extern void func_0018E5A8(void *);
extern void func_0013C128(Obj *,int,int);
extern int func_00139F50();
extern void func_0013A088(int);
extern void func_0018D8F8(void *,int,int,void *);
extern char *func_0018D860(void *);
extern void func_00254D68(char *,char *,...);
extern void func_0013B5E8(int,int,int,int);
typedef AnimationHandle Tmp;
void func_0013BA60(Obj *p) { Tmp tmp; char buffer[32];
 int ready, before, after;
 Manager *g; int *input;
 p->x46C=0; p->x464=D_0029C4B8; p->x460=D_0029C4BC;
 func_0013B9C0();
 g=&D_003F52D0;
 if(g->x150==p) {
  input=&g->x64; ready=0;
  func_0013C838(p,p->x528==0); func_00190358(g);
  Tmp *t=&tmp; t->x8=0; func_00203590(t,0x2588,0,p->x520);
  if(p->x524==1) { if(func_00203C28(t)==1.0f) ready=1; }
  if(p->x524==2 && func_00203C28(&tmp)==0.0f) p->x524=0;
  before=-1; after=-1;
  if(p->x500) before=p->x500->x0;
  if(p->x4A8>=0) {
   if(func_00190D60(input,p->x4A8,p->x45C)) {
    if(D_0029C471 && !p->x528) func_00190660(&D_003F52D0,D_00362990);
    else { p->v7(); func_001C57A8(D_004B2A90,(u128)0x4B4341425F53UL); }
   }
  }
  func_0018DBA8(p); func_00208500(D_003623A0); p->x4A8=5;
  if(!p->x524 && func_00190D60(input,8,p->x45C)) {
   int other=0, game;
   func_0013A170(); func_00140C38(D_0036B730);
   if(D_004B25A0.x10C==3) goto other_done;
   if(D_004B25A0.x10C==2) goto other_done;
   if(D_004B25A0.x10C==1) goto other_done;
   if(D_004B25A0.x10C==0) goto other_done;
   if(D_004B25A0.x10C==4) goto other_done;
   if(D_004B25A0.x10C==5) goto other_done;
   if(D_004B25A0.x10C==11) goto other_done;
   if(D_004B25A0.x10C==17) goto other_done;
   if(D_004B25A0.x10C==15) goto other_done;
   if(D_004B25A0.x10C==6) goto other_done;
   if(D_004B25A0.x10C==7) goto other_done;
   if(D_004B25A0.x10C==8) goto other_done;
   if(D_004B25A0.x10C!=9) other=1;
other_done:
   game=0;
   if(D_004B25A0.x10C==3) goto is_game;
   if(D_004B25A0.x10C==2) goto is_game;
   if(D_004B25A0.x10C==1) goto is_game;
   if(D_004B25A0.x10C==0) goto is_game;
   if(D_004B25A0.x10C==4) goto is_game;
   if(D_004B25A0.x10C==5) goto is_game;
   goto game_done;
is_game: game=1;
game_done:
   if(game) func_001C57A8(D_004B2A90,((u128)0x454D4147UL<<64)|(u128)0x5F5245544E455F53UL);
   else func_001C57A8(D_004B2A90,(u128)0x5443454C45535F53UL);
   if(other) {func_00190660(&D_003F52D0,D_004C7A30);return;}
   if(D_0029C471) {func_00190660(&D_003F52D0,D_00362990);return;}
   if(game) {func_00190660(&D_003F52D0,D_00362E90);return;}
   D_003F52D0.x12D=1;return;
  }
  func_00218498(func_0013AF60(0x25C7),1);
  func_00218498(func_0013AF60(0x2583),0);
  func_00218498(func_0013AF60(0x2598),1);
  if(!D_0029DD14) {D_0029DD14=1;D_0029DD10=D_004B25A0.x290;}
  func_00218498(func_0013AF60(0x2591),D_004B25A0.x290/32-1);
  func_001F57D0(0x2351,D_004B25A0.x290/32-1);
  if(!p->x524 || ready) func_0013C338(p);
  if(!p->x528) func_0013C5E0(p); else func_0013C6A8(p);
  if(func_00190D60(input,4,p->x45C)) p->v6(p->x0);
  p->x478=1;
  if(p->x500 && p->x500->v14(p->x500->x0) && func_0018D8B8(p->x500->v14(p->x500->x0))<2) p->x500->x478=0;
  else p->x500->x478=1;
  func_0018E5A8(p);
  if(p->x500) after=p->x500->x0;
  func_0013C128(p,before,after);
  if(func_00139F50()) func_0013A088(0);
  if(!p->x524) {
   if(!D_0029DD18) {InitializeUiStyleDiscard((UiStyle *)(void *)(D_00361690),256,130,-89,-33,90,90,90,128);D_0029DD18=1;}
   func_0018D800((const UiStyle *)(void *)(D_00361690)); func_0018D8F8(p,25,205,D_0029DD30->x44);
   if(!D_0029DD1C) {InitializeUiStyleDiscard((UiStyle *)(void *)(D_003616B0),256,130,-89,-33,90,90,90,128);D_0029DD1C=1;}
   func_0018D800((const UiStyle *)(void *)(D_003616B0)); func_0018D8F8(p,20,205,D_0029DD30->x48);
  }
  if(!p->x524 || ready) {
   func_0018D800((const UiStyle *)(void *)(D_00362468));
   if(!p->x528) func_00254D68(buffer,D_0029C4C0,func_0018D860(p->x504));
   else func_00254D68(buffer,D_0029C4C8,func_0018D860(p->x504));
   func_0018D8F8(p,13,12,buffer);
  }
  func_0013B5E8(0,0,0,0);
 }
}
