/* UNIT 0x0013C128 0x0013C300 .sbss=0x29DD20:0x29DD24 */
typedef struct Sound { int x0,x4,x8,xC; } Sound;
typedef struct Controller { char pad[0x44]; float x44; char tail[0x28]; } Controller;
typedef struct Obj { char pad[0x514]; int x514[3]; } Obj;
int D_0029DD20 __attribute__((section(".sbss"))) = 0;
extern Sound D_003616D0;
extern Controller D_00365790;
extern void func_00203590(Sound *,int,int,int);
extern void func_001AC7B0(Controller *,Sound *);
extern void func_001AC810(Controller *,float);
extern void func_001ACB70(Controller *,float);
extern void func_00203B68(Sound *,int,float);
extern void func_002039B0(Sound *,int);
extern void func_00204080(Sound *,int);
extern void func_00202E38(Sound *,int,int,float);
void func_0013C128(Obj *obj,int old,int current) {
 Sound sound; int direction,index;
 if (D_00365790.x44 == 0.0f) {
  if (!D_0029DD20) { D_003616D0.x8=0; D_0029DD20=1; }
  func_00203590(&D_003616D0,0x25C6,0,0);
  func_001AC7B0(&D_00365790,&D_003616D0);
  func_001AC810(&D_00365790,0.0f);
 }
 if (current == -1 || obj->x514[0]<0) func_001ACB70(&D_00365790,-1.0f);
 else func_001ACB70(&D_00365790,1.0f);
 if (current != -1) {
  if (old == -1) {
   sound.x8=0;
   func_00203590(&sound,0x25C6,0,obj->x514[current]);
   func_00203B68(&sound,0,0.0f);
   func_002039B0(&sound,0);
  } else if (old != current) {
   direction=0;
   if (old >= current) { if (old != 2 || current != 0) direction=1; }
   index = direction ? current : old;
   if ((unsigned int)index < 3 && obj->x514[index]>=0) {
    sound.x8=0;
    func_00203590(&sound,0x25C6,0,obj->x514[index]);
    func_00204080(&sound,direction ^ 1);
    func_00202E38(&sound,1,1,100.0f);
   }
  }
 }
}
