/* UNIT 0x001301E0 0x001304F8 .rodata=0x289080:0x2890A0 */
typedef struct Player { char pad[0x6610]; char x6610[0x1378]; unsigned int x7988; char pad798C[0xD0]; int x7A5C; char pad7A60[0x68]; float x7AC8,x7ACC,x7AD0,x7AD4,x7AD8; char pad7ADC[0xC4]; float x7BA0; } Player;
extern char D_0029C2A0[],D_0029C2A8[];
extern char D_003B2270[];
extern void func_00254D68(char*,char*,int,char*);
extern void func_0017B338(void*,int,int,int,int,char*);
extern void func_001E6420(void*,float);
static inline void animate(char *base,unsigned int index,int value,int effect,char *text) {
 switch(index) { case 0: case 1: case 2: case 3: func_0017B338((index*0x1380+0x150)+base,value,1,0,effect,text); break; }
}
void func_001301E0(Player *p,int kind) {
 char text[16]; int points=0; int enabled=p->x7A5C==1;
 switch(kind) {
 case 0:
  points=(int)(p->x7BA0*50.0f); p->x7ACC+=points; func_00254D68(text,D_0029C2A0,points,D_0029C2A8);
  if(enabled) animate(D_003B2270,p->x7988,25,7,text); break;
 case 1:
  points=(int)(p->x7BA0*100.0f); p->x7AD0+=points; func_00254D68(text,D_0029C2A0,points,D_0029C2A8);
  if(enabled) animate(D_003B2270,p->x7988,24,6,text); break;
 case 2:
  points=(int)(p->x7BA0*200.0f); p->x7AC8+=points; func_00254D68(text,D_0029C2A0,points,D_0029C2A8);
  if(enabled) animate(D_003B2270,p->x7988,23,11,text); break;
 case 3:
  points=(int)(p->x7BA0*300.0f); p->x7AD8+=points; func_00254D68(text,D_0029C2A0,points,D_0029C2A8);
  if(enabled) animate(D_003B2270,p->x7988,28,14,text); break;
 case 4:
  points=(int)(p->x7BA0*400.0f); p->x7AD4+=points; func_00254D68(text,D_0029C2A0,points,D_0029C2A8);
  if(enabled) animate(D_003B2270,p->x7988,30,15,text); break;
 }
 func_001E6420(p->x6610,(float)points);
}
