/* UNIT 0x0016D8C0 0x0016DAF0 */
struct Handle { int x0,x4,x8,xC; };
struct State { char pad0[8]; int x8; char padC[0x34]; int x40[60]; char *x130; };
struct Obj { char pad0[0x208]; char *x208; char pad20C[0x4A8]; int x6B4,x6B8; };
extern void func_00126B20(struct State *,int);
extern float func_001385A8(void *);
extern int func_00222BD8(int,int);
extern void func_00203DC8(struct Handle *,int);
extern float func_00203C28(struct Handle *);
extern int func_00204230(struct Handle *);
extern void func_002038E0(struct Handle *,int);
int func_0016D8C0(struct Obj *obj,struct State *state) {
 struct Handle a,b,c; int result=0; int st;
 if(state) {
  struct Handle *h;
  st=state->x8;
  { int off=state->x40[st]*16+16; h=(struct Handle *)(state->x130+off); }
  switch(st) {
  case 1:
   func_00126B20(state,6);
   st=state->x8;
   { int off=state->x40[st]*16+16; a=*(struct Handle *)(state->x130+off); }
   func_00203DC8(&a,0);
   break;
  case 6: {
   int done=0;
   a=*h;
   done=func_00203C28(&a)>=1.0f || (b=*h,func_00204230(&b)==0);
   if((unsigned char)done) {
    obj->x6B8++;
    if(obj->x6B8>=16) {
     int limit=4; obj->x6B8=0; obj->x6B4++;
     if(obj->x208) limit=(int)(func_001385A8(obj->x208+0x40C0)*4.0f);
     limit+=func_00222BD8(0,2);
     if(obj->x6B4<limit) { c=*h; func_002038E0(&c,1); }
     else { obj->x6B4=0; result=1; func_00126B20(state,1); }
    }
   }
   break;
  }
  }
 }
 return result;
}
