/* UNIT 0x00191FC0 0x00192118 .rodata=0x28D830:0x28D850 */
struct Pos { unsigned int *x0; char pad[0x14]; float height; };
struct Rider { char pad[0x792c]; struct Pos *pos; };
struct Global { char pad[0x501a0]; struct Rider *rider; };
struct Obj { int x0; unsigned int state; struct Pos *pos; char pad[0x68]; int timer; };
extern struct Global D_002AF2C0;
extern char D_003041B0[],D_003F5460[];
extern float func_00222330(void *,void *);
extern void *func_00122258(void *,unsigned int),*func_00191790(void *,void *);
extern void func_00192118(struct Obj *,void *),func_00191BC0(struct Obj *,int),func_001921C0(struct Obj *),func_00192268(struct Obj *),func_00192460(struct Obj *),func_001926E0(struct Obj *);
extern int func_00238F60(void);
void func_00191FC0(struct Obj *p) {
 struct Rider *r; void *value;
 if(p->pos) {
 r=D_002AF2C0.rider;
 if(r && func_00222330((char *)p->pos+0x10,(char *)r->pos+0x10)>1000.0f && r->pos->height<p->pos->height) {
 value=func_00122258(D_003041B0,*p->pos->x0>>18);
 value=func_00191790(D_003F5460,value);
 func_00192118(p,value);
 }
 }
 p->timer+=func_00238F60();
 switch(p->state) {
 case 0: if(p->pos) func_00191BC0(p,1); break;
 case 1: func_001921C0(p); break;
 case 2: func_00192268(p); break;
 case 3: func_00192460(p); break;
 case 4: func_001926E0(p); break;
 }
}
