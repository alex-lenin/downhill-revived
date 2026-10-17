/* UNIT 0x00111D98 0x00111F10 */
/* func_00111D98: update proximity flag (bit 54 of x440) for rider r vs obj p.
   Sets the bit when r is p->x8, enable bit (x3a0 bit 2) is set, the
   x792C->x18 positions are within 700.0, and r->x7958 matches one of
   p->x70/x74/x78 (or passes the func_00118CA8 / func_00111F10 checks);
   otherwise clears the bit. */
typedef struct Target { char pad0[8]; float x8; } Target;
typedef struct Pos { char pad0[0x18]; float x18; } Pos;
typedef struct Rider { char pad0[0x792c]; Pos *x792c; char pad7930[0x28]; Target *x7958; } Rider;
typedef struct Obj { Rider *x0; int x4; Rider *x8; char padc[0x64]; Target *x70,*x74,*x78; char pad7c[0x324]; int x3a0; char pad3a4[0x70]; int x414; char pad418[0x28]; unsigned long x440; } Obj;
extern struct { char pad0[0x10064]; int x10064; } D_0029EA20;
extern int func_00118CA8(void *,Target *,Target *,int),func_00111F10(Obj *);
void func_00111D98(Obj *p,Rider *r) {
 Target *t; float difference,adiff,limit,dummy;
 if(r!=p->x8) return;
 if(((p->x3a0>>2)&1)==0) goto clear;
 /* Zero-byte asm labels (cf. src/unsorted/0012F8D8.c): scheduling barriers
    only, they emit no bytes. They keep the 700.0f load below abs.s (retail
    order; otherwise the scheduler hoists it into the lwc1 load-delay slot)
    and shape FP allocation to load/sub $f0, abs $f2, const $f1. Plain C and
    do/while(0) cannot do both: pure C hoists the const early and reuses $f0
    for abs, while a loop swaps the load regs and pads via .p2align 3. */
 __asm__ volatile ("Lfdum_11D98:" : "=f" (dummy));
 difference=r->x792c->x18-p->x0->x792c->x18;
 __asm__ volatile ("LfuseD_11D98:" : : "f" (dummy));
 adiff=__builtin_fabsf(difference);
 __asm__ volatile ("LfuseV_11D98:" : : "f" (difference));
 limit=700.0f;
 if(!(limit>adiff)) goto clear;
 t=r->x7958;
 if(!t) return;
 if(p->x74==t || p->x70==t || p->x78==t) { p->x440|=1UL<<54; return; }
 if(t->x8<p->x74->x8) {
 D_0029EA20.x10064=0;
 if(func_00118CA8(&D_0029EA20,p->x74,t,0)) { p->x440|=1UL<<54; return; }
 if(p->x414<421) goto clear;
 p->x440=(p->x440 & ~(1UL<<54)) | ((unsigned long)(func_00111F10(p)&1)<<54);
 }
 return;
clear:
 p->x440 &= ~(1UL<<54);
}
