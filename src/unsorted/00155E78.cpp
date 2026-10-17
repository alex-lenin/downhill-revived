/* UNIT 0x00155E78 0x001564A8 .rodata=0x28A280:0x28A2B0 .sdata=0x29C6A8:0x29C6B0 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef struct Obj { char *x0; char pad4[0x20-4]; Vec x20; char pad30[0x78-0x30]; float x78,x7C,x80,x84; char pad88[0xAC-0x88]; int xAC,xB0; char padB4[8]; int xBC,xC0,xC4; unsigned long flags; } Obj;
typedef struct VEntry { short delta,pad; int (*call)(void *); } VEntry;
typedef struct Event { int x0; VEntry *vtable; } Event;
#define AT(T,p,o) (*(T *)((char *)(p)+(o)))
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
unsigned int D_0029C6A8 __attribute__((aligned(8))) = 0x80101010;
extern unsigned int D_0029C6B0;
typedef struct Sound { char pad0[0x2B8]; int x2B8,x2BC; int state() const { return x2B8; } char pad2C0[0xC0]; } Sound;
typedef struct Game { char pad0[0x10C]; int x10C; char pad110[0x254]; int x364; } Game;
extern Sound D_0037EA60[];
extern Game D_004B25A0;
extern char D_0029C6B8[],D_00398C40[],D_003B2270[];
extern int func_00111D30(void *),func_00148B68(void *,void *);
extern void func_001E65E8(void *),func_0012FE38(void *,void *),func_001D44D0(void *,void *);
extern void func_0020CC40(int,float,float,unsigned int,int);
extern void func_001C7890(void *),func_001308A8(void *),func_001A4F70(void *,int),func_00133980(void *,void *);
extern void func_00148560(void *,int),func_0012DE68(void *,int);
extern char *func_00158650(int);
extern char *func_00158668(int);
extern char *func_00254544(char *,const char *);
extern void func_002542C4(void *,char *);
extern void func_0017B338(void *,int,int,int,int,void *);
extern void func_00199A90(void *),func_001CBA18(void *,int),func_00158520(void *,float);
static inline Sound *sound_at(Sound *base,unsigned int index) { return (Sound *)(index*0x380+(char *)base); }
static inline void position(char *base,void *p) { func_001D44D0(base+0x240,p); }
static inline void animate(char *base,unsigned int index,int value,int effect,char *text) {
 switch(index) { case 0: case 1: case 2: case 3: func_0017B338((index*0x1380+0x150)+base,value,1,1,effect,text); break; }
}
void func_00155E78(Obj *self,int kind) {
 char name[32];
 int sound;
 float duration;
 char *rider=self->x0;
 if(AT(void *,rider,0x4ECC)==rider+0x6320) return;
 if(self->flags&4) return;
 if(AT(int,rider,0x7A5C)==3 && AT(void *,rider,0x78D4) && !func_00111D30(AT(void *,rider,0x78D4))) return;
 self->xAC=kind;
 self->flags=(self->flags|4)&~8L;
 func_001E65E8(self->x0+0x6610);
 {
  Event *event=(Event *)(self->x0+0x4EE0);
  if(event->vtable[4].call((char *)event+event->vtable[4].delta)) func_0012FE38(self->x0,self->x0+0x4EE0);
 }
 duration=0.25f;
 sound=func_00148B68(D_0037EA60,self->x0);
 if(AT(int,self->x0,0x79A0)) {
  if(sound>=0) {
   position(D_00398C40,AT(char *,self->x0,0x792C)+0x10);
   func_0020CC40(sound,0.1875f,0.0625f,D_0029C6B0,AT(int,self->x0,0x3514));
  }
 } else {
  switch(self->xAC) {
   case 4: duration=0.25f; break;
   case 1: duration=self->x78; break;
   case 2: case 5: case 7: case 8: case 9: duration=self->x7C; break;
   case 6: duration=self->x84; break;
   case 3: func_001C7890(self->x0+0x7720);
   default: duration=self->x80; break;
  }
  if(sound>=0) {
   int special=0;
   if(D_004B25A0.x10C==15) special=D_004B25A0.x364==1;
   if(!special || AT(int,self->x0,0x79AC)<=0) func_0020CC40(sound,duration-0.0625f,0.0625f,D_0029C6A8,0);
  }
 }
 if(self->xBC<7 && kind!=6) { self->xB0=0; self->flags|=0x80; }
 else { self->flags&=~0x80L; self->xB0=(int)(duration*60.0f); }
 func_001308A8(self->x0);
 COPY_VECTOR(self->x20,AT(char *,self->x0,0x792C)+0x10);
 func_001A4F70(self->x0+0x4EF0,self->xAC==6);
 func_00133980(self->x0+0x6420,self->x0);
 if(AT(int,self->x0,0x7984) && D_0037EA60[AT(unsigned int,self->x0,0x7988)].state()!=4) {
  if((self->flags&2) && kind!=3) func_00148560(&D_0037EA60[AT(unsigned int,self->x0,0x7988)],2);
  else func_00148560(&D_0037EA60[AT(unsigned int,self->x0,0x7988)],3);
  if(!sound_at(D_0037EA60,AT(unsigned int,self->x0,0x7988))->x2BC) func_0012DE68(self->x0,0);
  if(self->xAC==5) {
   func_00254544(name,(char *)func_00158650(AT(int,self->x0,0x79A8)));
   if(AT(int,self->x0,0x79A8)==0xC0) func_00254544(name,func_00158668(5));
   func_002542C4(name,D_0029C6B8);
   animate(D_003B2270,AT(unsigned int,self->x0,0x7988),6,15,name);
  } else if(self->xAC==6) {
   animate(D_003B2270,AT(unsigned int,self->x0,0x7988),7,0,0);
  } else if(self->xAC==7) {
   animate(D_003B2270,AT(unsigned int,self->x0,0x7988),8,0,0);
  } else if(self->xAC==8) {
   func_00254544(name,(char *)func_00158650(AT(int,self->x0,0x79A8)));
   if(AT(int,self->x0,0x79A8)==0xC0) func_00254544(name,func_00158668(5));
   func_002542C4(name,D_0029C6B8);
   animate(D_003B2270,AT(unsigned int,self->x0,0x7988),29,11,name);
  } else if(self->xAC==9) {
   func_00254544(name,(char *)func_00158650(AT(int,self->x0,0x79A8)));
   if(AT(int,self->x0,0x79A8)==0xC0) func_00254544(name,func_00158668(5));
   func_002542C4(name,D_0029C6B8);
   animate(D_003B2270,AT(unsigned int,self->x0,0x7988),31,15,name);
  }
 }
 AT(int,self->x0,0x7998)++;
 if(kind==3) self->xC0=0;
 if(AT(void *,self->x0,0x78D0)) func_00199A90(AT(void *,self->x0,0x78D0));
 func_001CBA18(self->x0+0x7720,kind);
 switch(kind) { case 2: case 6: case 7: if(AT(int,self->x0,0x79A0)<=0) func_00158520(self->x0+0x40A0,1.0f); break; }
}
