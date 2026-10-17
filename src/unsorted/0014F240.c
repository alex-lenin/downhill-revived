/* UNIT 0x0014F240 0x0014F300 */
struct Obj {char pad[0x500];int x500,x504,x508,x50C;union {struct {int lo,hi;} words;long value;} x510;};
struct Other {char pad[0x796C];int x796C;};
struct Global {char pad[0x50130];struct Other *x50130;};
extern struct Global D_002AF2C0;
extern char D_003F52D0[],D_004225A0[];
extern int D_004B26AC[];
extern int func_0018DB08(struct Obj *);
extern int func_001B5AF0(void *,int,int);
extern int func_00194E30(void *,int);
extern int func_0014F868(struct Obj *);
void func_0014F240(struct Obj *p) {
 struct Other *q;
 func_0018DB08(p);func_001B5AF0(D_003F52D0,1,1);
 p->x500=480;p->x508=2;p->x504=0;p->x510.words.lo=0;p->x510.words.hi=0;
 q=D_002AF2C0.x50130;
 if(func_00194E30(D_004225A0+((q->x796C>>5)-1)*0x694,D_004B26AC[0])==3)func_0014F868(p);
 if(p->x50C==0 && p->x510.value==0)p->x500=0;
}
