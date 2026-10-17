/* UNIT 0x0014E460 0x0014E608 */
#define F(p,t,o) (*(t *)((char *)(p)+(o)))
struct Global { char pad[0x50130]; void *x50130; };
struct State { char pad[0x10c]; int x10c; char pad2[0x178]; int x288; };
extern struct Global D_002AF2C0;
extern struct State D_004B25A0;
extern char D_00398C40[],D_004225A0[];
extern void func_0014E608(void *),func_0014EB08(void *);
extern int func_001947B0(void *),func_00194E30(void *,int),func_00194EF8(void *);
extern void func_001C17B8(void *,int,int);
static __inline__ void notify(void *p,int value,int flag) { func_001C17B8((char *)p+0x5b74,value,flag); }
void func_0014E460(void *p) {
 int mode=D_004B25A0.x288; int old;
 if(mode==1) {
 F(p,int,0x14)=0;
 old=func_00194E30(D_004225A0+((F(D_002AF2C0.x50130,int,0x796c)>>5)-1)*0x694,D_004B25A0.x10c);
 func_0014EB08(p);
 if(F(p,int,8)) {
 F(p,int,12)=func_00194EF8(D_004225A0+((F(D_002AF2C0.x50130,int,0x796c)>>5)-1)*0x694);
 func_0014E608(p);
 if(old!=func_00194E30(D_004225A0+((F(D_002AF2C0.x50130,int,0x796c)>>5)-1)*0x694,D_004B25A0.x10c)) F(p,int,0x14)=mode;
 }
 notify(D_00398C40,F(D_002AF2C0.x50130,int,0x798c),0);
 return;
 } else if(mode==2) {
 func_0014EB08(p);
 if(F(p,int,8)) { F(p,int,12)=func_001947B0(D_004225A0); func_0014E608(p); }
 notify(D_00398C40,F(D_002AF2C0.x50130,int,0x798c),0);
 return;
 }
}
