/* UNIT 0x00141188 0x001413D0 */
#include "timer.h"
typedef struct Name { int x0,x4,x8; } __attribute__((packed,aligned(4))) Name;
typedef struct State {
char pad0[0x18]; Name x18,x24;
char x30[0x18],x48[0x18],x60[0x30];
unsigned char x90,x91; char pad92[2]; int x94[2],x9C,xA0,xA4;
char xA8[16],xB8[16],xC8[16]; char padD8[0x30];
unsigned char x108; char pad109[0x1B]; int x124,x128;
unsigned char x12C; char pad12D[3]; int x130;
} State;
typedef struct Global { char pad0[0x1680]; int x1680; void *x1684; } Global;
extern Global D_004D0380,D_004D1A18,D_004D30B0,D_004D4748;
extern char D_002AF2C0[],D_002618D8[],D_00759780[];
extern void func_00119E60(void *);
extern void func_0013B328(void);
extern void func_001410B8(State *);
extern int func_001FA3E8(Name *);
extern int func_001FA4B0(Name *,int);
extern void func_00206A20(int);
extern void func_00208118(void *);
extern void func_00223AD8(void *,int,int);
extern int func_00254544(void *,int);
void func_00141188(State *self,Name *a,Name *b) {
int i, text; void *slot;
self->x18=*a; self->x24=*b;
func_00119E60(D_002AF2C0);
if(self->x130<0) self->x130=D_004D30B0.x1680;
if(!self->x94[0]) func_001410B8(self);
func_0013B328();
slot=self->x48;
for(i=0;i<5;i++) func_00206A20(self->x130+i);
self->xA4=3;
if((unsigned)((int)b->x0-35)<3U) self->xA4=2;
text=func_001FA3E8(a);
func_00254544(self->xA8,text);
text=func_001FA4B0(b,1);
func_00254544(self->xB8,text);
text=func_001FA4B0(b,2);
func_00254544(self->xC8,text);
if(self->x91) { self->x91=0; self->xA0=!self->xA0; }
func_00223AD8(self->x30,self->x94[self->xA0],0x9B000);
func_00223AD8(slot,self->x9C,0x20000);
D_004D0380.x1684=self->x30;
D_004D30B0.x1684=self->x30;
D_004D1A18.x1684=self->x30;
D_004D4748.x1684=self->x30;
self->x124=0x3F80; self->x128=0; self->x108=0;
if(self->x12C) func_00208118(self->x60);
else { func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(D_002618D8), self, 1); self->x12C=1; }
}
