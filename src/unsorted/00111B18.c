/* UNIT 0x00111B18 0x00111D30 */
typedef struct Rider { char pad0[0x796c]; int x796C; char pad7970[0x1c]; int x798C; char pad7990[0xd8]; unsigned char x7A68; } Rider;
typedef struct Obj { Rider *x0; int x4; Rider *x8; char padC[0x64]; void *x70,*x74; char pad78[0x328]; int x3A0; char pad3A4[0x64]; int x408,x40C; char pad410[0x2c]; int x43C; unsigned long x440; } Obj;
extern void func_00111F98(Obj *,int *);
extern int func_00112158(Obj *);
extern int D_0039E7F4[];
extern int D_0043EEC0[];
void func_00111B18(Obj *obj) {
 int time, duration, remaining, offset;
 if(obj->x0->x7A68==0) {
  obj->x408=0; obj->x40C=0;
  if(obj->x74 && obj->x70) {
   func_00111F98(obj,&time);
   obj->x43C=func_00112158(obj);
   if((obj->x3A0>>11)&1) time-=180;
   else if(obj->x440 & ((unsigned long)1<<56)) time-=120;
   else if(obj->x0->x796C==0x1c0 || obj->x0->x796C==0x1e0) {
    time+=180;
    if(obj->x0->x798C<4) time+=420;
   }
   duration=obj->x43C;
   offset=time-D_0043EEC0[2];
   obj->x408=offset;
   remaining=duration-D_0043EEC0[2];
   if(remaining<0) remaining=0;
   obj->x40C=remaining;
   if(duration!=0) {
    if(D_0039E7F4[0]==0) obj->x408=offset+remaining*360/duration;
    else if(D_0039E7F4[0]==1) obj->x408=offset+remaining*120/duration;
    else obj->x408=offset-remaining*120/duration;
   }
   if(obj->x8 && obj->x0->x798C<obj->x8->x798C) obj->x408+=(obj->x0->x798C/2)*60;
   if(obj->x408>0 && (float)obj->x43C>0.0f) obj->x408=obj->x408*D_0043EEC0[2]/obj->x43C;
  }
 }
}
