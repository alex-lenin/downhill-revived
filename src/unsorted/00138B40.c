/* UNIT 0x00138B40 0x00138E08 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00139940_3_func_00139940 func_00139940
#else
#define CallScalarBridge00139940_3_func_00139940 ScalarBridge00139940_3
#endif
typedef unsigned int Vec __attribute__((mode(TI)));
typedef struct Packet { int x0; char *x4; } Packet;
typedef struct Rider { char pad0[0x792C]; char *x792C; char pad7930[0x12C]; int x7A5C; } Rider;
typedef struct State {
 char pad0[0x2BF10]; int x2BF10; char pad2BF14[0x57CC];
 Packet *x316E0, *x316E4;
 int x316E8,x316EC,x316F0,x316F4,x316F8,x316FC,x31700,x31704;
 Rider *x31708; int x3170C,x31710,x31714; long *x31718;
 char pad3171C[0x34]; Vec x31750; int x31760;
} State;
extern struct Globals { char pad0[0x10C]; int x10C; char pad110[0x178]; int x288; } D_004B25A0;
extern int D_004C3778[];
extern void func_001389D0(State *);
extern void func_00139710(State *, Vec *);
extern long *func_001591D8(int);
extern void func_001398F8(State *);
extern void func_00138828(State *, Vec *);

#if defined(__mips__)
extern void func_00139940(State *, char *, int, int, float);
#else

#endif

#define COPY_VECTOR(dst, src) __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
void func_00138B40(State *s, Rider *r) {
 int mode, index; char *offset; Vec vec; Vec *dst; long mask;
 if(r->x7A5C != 1) return;
 mode = D_004B25A0.x288;
 if(mode != r->x7A5C || D_004B25A0.x10C != 0x12) return;
 s->x31700=0; s->x31704=0; s->x316E8=0; s->x316EC=0;
 s->x316F0=0; s->x316F4=0; s->x316F8=0; s->x316FC=0;
 s->x31708=r; func_001389D0(s);
 if(s->x31760 < 9) {
  s->x2BF10=0xAF0;
  if(D_004C3778[0] == mode) s->x316E4=(Packet *)((char *)s+0x2EB00);
  else if(D_004C3778[0] == 0) s->x316E4=(Packet *)((char *)s+(s->x31760*0x2BE0+0x10));
  else s->x316E4=0;
 } else {
  if(s->x31760 >= 24) return;
  s->x2BF10=0x578;
  if(D_004C3778[0] == mode) s->x316E4=(Packet *)((char *)s+0x2EB00);
  else if(D_004C3778[0] == 0) s->x316E4=(Packet *)((char *)s+(s->x31760*0x1600+0xAF10));
  else s->x316E4=0;
 }
 s->x316E0->x4=(char *)s+(s->x2BF10*4+0x2BF40);
 s->x316E0->x4-=0x10; s->x316E0->x0=0;
 if(s->x316E4) {
  s->x316E4->x4=(char *)s->x316E4+(s->x2BF10*4+0x20);
  s->x316E4->x4-=0x10;
  dst=&s->x31750;
  COPY_VECTOR(*dst,s->x316E4->x4);
  func_00139710(s,dst);
 }
 s->x31710=3; s->x316E0->x0=0;
 if(s->x31714 && s->x316E4) {
  index=0; s->x31718=func_001591D8(s->x31714);
  mask=(long)0xEFFFFFFFFFFFFFFFUL;
  *s->x31718 &= mask;
  func_001398F8(s);
  vec=s->x31750; func_00138828(s,&vec);
  vec=s->x31750; func_00138828(s,&vec);
  while(index < s->x2BF10) {
   CallScalarBridge00139940_3_func_00139940(s,s->x31708->x792C+0x10,0x404040,index,0.0f); index++;
  }
 }
}
