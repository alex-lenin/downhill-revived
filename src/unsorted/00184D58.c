/* UNIT 0x00184D58 0x00184E30 */
struct State { int x0,x4,x8,xC,x10; };
extern int func_002460F8(int,int,int,int);
extern int func_002466A0(int,int *,int *);
extern int func_002464B8(int,int,int);
extern int func_00246258(int);
extern void func_00184938(struct State *,int);
int func_00184D58(struct State *self,struct State *state) {
 int command,handle,result;
 if(!state->x0) {
  func_002460F8(state->x10,state->xC,self->x8,0x203);
  command=2; func_002466A0(0,&command,&handle);
  if(handle==-1) goto error;
  func_002464B8(handle,self->xC,self->x10);
  command=6; func_002466A0(0,&command,&result);
  if(result>=0) {
   func_00246258(handle); command=3; func_002466A0(0,&command,&result); return 1;
  } else {
error: func_00184938(state,state->x4);
  }
 }
 return 1;
}
