/* UNIT 0x001772B0 0x00177628 */
typedef struct { char pad0[0x1178]; unsigned char *x1178; char pad117C[0x104]; int x1280; char pad1284[0xE0]; int x1364; } Obj;
typedef struct { char pad0[0x10C]; int x10C; char pad110[0x178]; int x288; char pad28C[0xD8]; int x364; } State;
typedef struct { char pad0[0x79AC]; int x79AC; char pad79B0[0xAC]; int x7A5C; char pad7A60[0x170]; } Player;
typedef struct { Player players[10]; char pad4D620[0x2B88]; int count; } Players;
typedef struct { char pad0[0x488]; int x488; } Mode;
extern State D_004B25A0;
extern Players D_002AF2C0;
extern Mode D_0043EEC0;
extern int D_004B1F40[];
extern char D_0029C968[],D_0029C970[],D_0029C978[],D_0029C980[];
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F588(int,int,int,int,int,int);
extern void func_0020F9A8(int,int,int,int,char *);
extern int func_00254D68(char *,const char *,...);
extern void func_0012F560(Player *,char *);
extern void func_001778D0(Obj *),func_00177AC8(Obj *),func_00178730(Obj *),func_00177770(Obj *),func_00177628(Obj *),func_00179338(Obj *),func_00179A60(Obj *),func_0017B3C0(Obj *),func_0017BA30(Obj *);
extern void func_00175730(Obj *,int);
static inline int greater(int limit,int value) { return limit<value; }
void func_001772B0(Obj *obj) {
 char text[32];
 int active,y,index,value;
 Player *player;
 active=0;
 if(D_004B25A0.x10C==15) active=D_004B25A0.x364==1;
 if(active) {
  if(obj->x1364==0) {
   if(D_004B25A0.x288>=2) func_0020FB80(0,192,160,-67,-32);
   else func_0020FB80(0,224,192,-75,-32);
   func_0020F588(obj->x1280,0,110,110,200,190);
   y=42;
   func_0020F9A8(0,obj->x1280,10,30,D_0029C968);
   index=0;
   func_0020F9A8(0,obj->x1280,65,30,D_0029C970);
   for(;index<D_002AF2C0.count;index++) {
     player=&D_002AF2C0.players[index];
     func_0012F560(player,text);
     if(player->x7A5C==1) func_0020F588(obj->x1280,0,88,152,73,190);
     else func_0020F588(obj->x1280,0,80,100,80,190);
     func_0020F9A8(0,obj->x1280,10,y,text);
     value=player->x79AC;
     if(greater(1,value)) {
      func_0020F588(obj->x1280,0,80,20,20,170);
      func_0020F9A8(0,obj->x1280,80,y,D_0029C978);
     } else {
      func_00254D68(text,D_0029C980,value);
      func_0020F9A8(0,obj->x1280,80,y,text);
     }
     y+=10;
   }
  }
 } else if(obj->x1364==0) {
  func_00254D68(text,D_0029C980,D_004B1F40[0]/60);
  func_0020F588(obj->x1280,0,221,219,10,170);
  func_0020FB80(0,352,256,-91,-32);
  func_0020F9A8(0,obj->x1280,10,40,text);
 }
 if(obj->x1178) obj->x1178[4]=0;
 func_001778D0(obj); func_00177AC8(obj); func_00178730(obj);
 if(D_004B25A0.x288>=3) { func_00177770(obj); func_00177628(obj); }
 func_00179338(obj);
 if((unsigned int)(D_0043EEC0.x488-2)<3) func_00179A60(obj);
 func_00175730(obj,1); func_0017B3C0(obj); func_0017BA30(obj);
}
