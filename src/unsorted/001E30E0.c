/* UNIT 0x001E30E0 0x001E3280 .sbss=0x29DE90:0x29DE98 */
#include "ui_style.h"
typedef struct State { int x0; char p4[4]; void *x8; char pC[0x40C]; int x418; char p41C[0x44]; int x460; char p464[0x44]; int x4A8; char p4AC[0x5C]; int x508; } State;
typedef struct Menu { char p0[0x500]; int x500; char p504[0x1C]; unsigned char x520[4][13]; unsigned char x554[1]; } Menu;
int D_0029DE90[2] __attribute__((section(".sbss")))={0,0};
extern Menu D_004C37A0;
extern char D_004C3780[],D_003F52D0[],D_0029DE68[],D_00293AC0[];
extern char *D_0026EFE8[];
extern void func_001B5AF0(void *,int,int),func_0018E638(State *,int,void *),func_0018E408(State *,void *,int),func_0018E6B8(State *,int,void *),func_0018DB08(State *);
void func_001E30E0(State *s) {
 Menu *menu;
 int count,i;
 unsigned char selected;
 unsigned char *items;
 char **names;
 s->x460=50; s->x508=-1;
 func_001B5AF0(D_003F52D0,1,1);
 
 selected=D_004C37A0.x554[D_004C37A0.x500];
 count=selected+1;
 if (!D_0029DE90[0]) {
  InitializeUiStyleDiscard((UiStyle *)(void *)(D_004C3780),270,160,-90,-33,56,56,56,255);
  D_0029DE90[0]=1;
 }
 s->x8=D_004C3780;
 func_0018E638(s,119,D_0029DE68);
 s->x0=selected; s->x418=0;
 for(i=0;i<count;i++) {
  unsigned char item=D_004C37A0.x520[D_004C37A0.x500][i];
  if(item==33) func_0018E408(s,D_00293AC0,1);
  else func_0018E408(s,D_0026EFE8[item],1);
 }
 s->x4A8=5;
 func_0018E6B8(s,122,D_0029DE68);
 func_0018DB08(s);
}
