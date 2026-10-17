/* UNIT 0x001E6970 0x001E6BC8 .sdata=0x29D748:0x29D750 */
__asm__(".extern D_0029D744, 4");
__asm__(".extern D_0029D750, 4");
typedef struct Rider { char pad0[0x7978]; int x7978; char pad1[0x7a5c-0x797c]; int x7a5c; } Rider;
typedef struct Obj { int x0,x4; char pad8[0x40-8]; Rider *x40; char pad44[0x720-0x44]; int x720,x724; int x728[300]; int xBD8[300]; int x1088[20]; int x10D8; char pad10DC[4]; unsigned char x10E0,x10E1; } Obj;
extern int D_0029D744;
int D_0029D748=3;
int unk_0029D74C=2;
extern int D_0029D750;
extern int func_00138560(void *);
extern int func_001E51A8(void);
extern void *func_002205A0(int);
extern int func_00220230(void *);
extern void func_002204A8(void *);
extern void func_001E6BC8(Obj *,int);
extern int func_001E6D10(Obj *,int);
extern void func_001E68A0(Obj *);
extern int func_001E7A00(void *);
extern int func_001E6370(Obj *);
extern void func_001C79C0(void *);
/* Empty branches: flow deletes their dead stores and jump2 deletes the jumps after
   register allocation, so they emit nothing, but the live ranges they add steer gcc's
   allocation order (bd8 base a1, on a2, previous a3, x728 base t0, axis t1). */
#define IDLE(n) if (buttons || obj) dzX = (n)
#define IDLE_4(n) IDLE(n); IDLE(n + 1); IDLE(n + 2); IDLE(n + 3)
#define IDLE_7 IDLE_4(1); IDLE(5); IDLE(6); IDLE(7)
#define IDLE_8 IDLE_4(1); IDLE_4(5)
#define IDLE_12 IDLE_4(1); IDLE_4(5); IDLE_4(9)
#define ABS(x) ((x)<0?-(x):(x))
int func_001E6970(Obj *obj,int mode) {
 unsigned char on; int result=0,buttons,previous,changed,analog; void *input; int stick=0,stickCopy,stickX=0,stickY=0,dzX,axis;
 if(!(func_00138560((char *)obj->x40+0x40c0)&0xff) && !func_001E51A8()) return 0;
 if(obj->x40->x7a5c!=1) return 0;
 input=func_002205A0(obj->x40->x7978); buttons=func_00220230(input); func_002204A8(input);
 stickCopy=stick; previous=obj->x10D8; obj->x10D8=previous+1; if(obj->x10D8>=300) obj->x10D8=0; IDLE_7;
 if(mode) buttons&=0x1000; else buttons&=0x1f01;
 obj->x728[obj->x10D8]=buttons; obj->xBD8[obj->x10D8]=stickY; dzX=stickX; axis=stickCopy; if(stickX>D_0029D750 || stickX< -D_0029D750) dzX=0; if(stickY>D_0029D750 || stickY< -D_0029D750) axis=0;
 if(buttons || axis) {
  IDLE_12; on=1; obj->x10E0=on;
  changed=obj->x728[previous]^buttons; analog=obj->xBD8[previous]^axis; IDLE_8; if(axis || obj) dzX=9;
  if(changed || analog) {
   if((changed & buttons) || analog) { obj->x720=0; if(!obj->x10E1) {obj->x10E1=on; obj->x724=0;} }
   else {obj->x720=0; if(obj->x10E1) func_001E6BC8(obj,0);}
  } else { if(obj->x10E1) obj->x724++; obj->x720++; }
 } else { if(obj->x10E1) obj->x724++; obj->x720++; }
 if(obj->x724>D_0029D744) func_001E6BC8(obj,1);
 if(obj->x10E0 && obj->x720>D_0029D748) {result=func_001E6D10(obj,mode); func_001E68A0(obj);}
 obj->x4=result;
 if((unsigned int)(result-29)<12) {obj->x4=func_001E7A00((char *)obj->x40+0x5ff0); if(!func_001E6370(obj)) obj->x4=0; else func_001C79C0((char *)obj->x40+0x7720);}
 return obj->x4;
}

