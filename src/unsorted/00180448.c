/* UNIT 0x00180448 0x001805B0 */
struct Position { char pad[0x18]; float x18; };
struct State { int x0,x4; struct Position *x8; char padC[0xC8]; int xD4; char padD8[0xD4]; float x1AC; };
struct World { char pad[0x5019C]; float x5019C; };
extern struct World D_002AF2C0;
extern float D_0029CAB0[];
extern int func_0011C0D0(void *,void *,int,float);
extern int func_0011B180(void *,void *,float *,int,float);
extern void func_0017FFB8(struct State *,int);
void func_00180448(struct State *s) {
 struct Position *p; int result;
 p=s->x8;
 if(p) {
  if(D_002AF2C0.x5019C+600.0f < p->x18) {
   if(s->x4==2) func_0017FFB8(s,1);
  } else {
   s->x1AC=D_0029CAB0[0];
   if(func_0011C0D0(&D_002AF2C0,(char *)p+0x10,0,900.0f)) {
    result=func_0011B180(&D_002AF2C0,(char *)s->x8+0x10,&s->x1AC,0,700.0f);
    s->xD4=result;
    if(result && s->x1AC<250000.0f && s->x4==1) func_0017FFB8(s,2);
   } else if(D_002AF2C0.x5019C+600.0f<s->x8->x18) {

    func_0017FFB8(s,1);
   } else if(s->x4==2) func_0017FFB8(s,1);
  }
 }
}
