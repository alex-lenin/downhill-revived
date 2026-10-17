/* UNIT 0x00210868 0x00210940 */
typedef struct { char p0[0x30]; int x30; unsigned int *x34; int x38; } Obj;
typedef struct { int x0,x4,x8,xC; } State;
extern int func_0016B5A8(void),func_00238F30(void);
extern void func_00203590(State *,unsigned int,int,int);
extern void func_00203B68(State *,int,float);
extern void func_00207E68(Obj *,void *,State *);
extern void func_00209248(int,State *,void *,float);
extern void func_00211280(int);
void func_00210868(Obj *p) {
char buffer[0x40]; State state; State handle;
if(p->x30) {
handle.x8=0;
func_00203590(&handle,*p->x34>>18,0,0);
if(!p->x38 && func_0016B5A8()) {func_00203B68(&handle,1,0.0f);p->x38=1;}
func_00207E68(p,buffer,&state);
func_00209248(p->x30,&state,buffer,1.0f/(float)func_00238F30());
func_00211280(p->x30);
}
}
