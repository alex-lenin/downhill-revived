/* UNIT 0x0014E6A0 0x0014E9D0 */
typedef struct Record {char pad[0x173]; char x173; char tail[0x694-0x174];} Record;
typedef struct Event {char pad[0x33]; char x33; char tail[0x50-0x34];} Event;
typedef struct Arg {int x0; int x4; char pad8[8]; int x10;} Arg;
typedef struct Rider {char pad[0x796C]; int x796C; int Index() {return (x796C>>5)-1;} } Rider;
typedef struct State {char pad0[0x12110]; int x12110; char pad12114[0x6C]; int x12180; char pad12184[0x17]; unsigned char x1219B; char pad1219C; unsigned char x1219D; Record *GetRecord(int i) {return &((Record *)this)[i];} } State;
extern State D_004225A0;
extern struct {char pad[0x50130]; Rider *x50130; Rider *x50134;} D_002AF2C0;
extern struct {char pad[0xA24]; int xA24; int xA28;} D_00380D00;
extern unsigned char D_00381717[], D_00381719[];
extern int D_0039E7F4[];
extern struct {char pad[0x288]; int x288;} D_004B25A0;
extern int func_00194770(State *,int);
extern void func_00194290(State *,int);
void func_0014E6A0(Arg *arg) {
 int flags=D_004225A0.x12110;
 int unlocked=D_004225A0.x12180;
 if(arg->x4==3) {
 int index=D_002AF2C0.x50130->Index();
 if(((unlocked>>index)&1)==0) {
 int mask=1<<index;
 D_004225A0.x12180=unlocked|mask;
 D_00380D00.xA24|=mask;
 }
 if(D_004225A0.x1219D==0) {D_004225A0.x1219D=1;D_00381719[0]=1;}
 if(func_00194770(&D_004225A0,arg->x4)>0 && ((flags>>10)&1)==0) {
 D_00380D00.xA28|=0x400;
 func_00194290(&D_004225A0,10);
 }
 } else if(arg->x4==4) {
 if(D_004B25A0.x288==1) {
 if(!D_004225A0.x1219B) {D_004225A0.x1219B=1;D_00381717[0]=1;}
 if(((flags>>7)&1)==0) {D_00380D00.xA28|=0x80;func_00194290(&D_004225A0,7);}
 } else if(D_004B25A0.x288==2) {
 ((Record *)&D_004225A0)[D_002AF2C0.x50134->Index()].x173=1;
 if(((flags>>7)&1)==0) {D_00380D00.xA28|=0x80;func_00194290(&D_004225A0,7);}
 }
 } else {
 if(arg->x10>=0 && ((flags>>arg->x10)&1)==0) {
 D_00380D00.xA28|=1<<arg->x10;
 func_00194290(&D_004225A0,arg->x10);
 }
 if(arg->x4==0 && D_0039E7F4[0]==2 && ((flags>>9)&1)==0) {
 D_00380D00.xA28|=0x200;func_00194290(&D_004225A0,9);
 }
 }
 if(D_004B25A0.x288==1) {
 int index=D_002AF2C0.x50130->Index();
 if(((unlocked>>index)&1)==0) {
 int mask=1<<index;
 D_00380D00.xA24|=mask;D_004225A0.x12180|=mask;
 }
 }
 ((Event *)D_004225A0.GetRecord(D_002AF2C0.x50130->Index()))[arg->x4].x33=1;
}
