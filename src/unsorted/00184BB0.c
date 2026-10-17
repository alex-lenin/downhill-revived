/* UNIT 0x00184BB0 0x00184D20 .rodata=0x28C610:0x28C630 .sdata=0x29CD4C:0x29CD50 .sbss=0x29DDE8:0x29DDF0 */
typedef struct State { unsigned int x0; int x4,x8,xC,x10; } State;
int D_0029CD4C=2;
int D_0029DDE8 __attribute__((section(".sbss"))) = 0;
int D_0029DDEC __attribute__((section(".sbss"))) = 0;
extern int func_002460F8(int,int,int,int);
extern int func_002466A0(int,int *,int *);
extern int func_002463A0(int,int,int);
extern int func_00246258(int);
extern void func_00184938(State *,int);
int func_00184BB0(State *state, State *other) {
 if(other->x0) return 1;
 switch(state->x0) {
 case 0:
  func_002460F8(other->x10,other->xC,state->x8,1);
  state->x0++;
 case 1:
  if(func_002466A0(1,&D_0029CD4C,&D_0029DDE8)) {
   if(D_0029DDE8==-1) goto error;
   goto advance;
  }
  break;
 case 2:
  func_002463A0(D_0029DDE8,state->xC,state->x10);
  state->x0++;
 case 3:
  D_0029CD4C=5;
  if(func_002466A0(1,&D_0029CD4C,&D_0029DDEC)) {
   if(D_0029DDEC<state->x10) {
error:
    func_00184938(other,other->x4); return 1;
   }
advance:
   state->x0++;
  }
  break;
 case 4:
  func_00246258(D_0029DDE8);
  state->x0++;
 case 5:
  D_0029CD4C=3;
  if(func_002466A0(1,&D_0029CD4C,&D_0029DDEC)) {
   if(D_0029DDEC<0) func_00184938(other,other->x4);
   return 1;
  }
  break;
 }
 return 0;
}
