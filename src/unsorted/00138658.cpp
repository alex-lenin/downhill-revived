/* UNIT 0x00138658 0x00138808 */
typedef struct Input { char p0[0x1198]; int x1198,x119C; char p1[0x1260-0x11A0]; int x1260,x1264; char p2[0x13D9-0x1268]; unsigned char x13D9,x13DA; char p3[0x6D28-0x13DB]; int x6D28; char p4[0x7BB1-0x6D2C]; unsigned char x7BB1; } Input;
typedef struct Obj { char p0[0x158]; Input *x158; char p1[0x488-0x15C]; float x488; char p2[8]; int x494; float x498; char p3[8]; short x4A4; } Obj;
extern int func_00138560(Obj *);
extern void func_00138808(Obj *);
static inline int right(Input *p) { if(p->x13DA) return p->x1264; return p->x119C; }
static inline int left(Input *p) { if(p->x13D9) return p->x1260; return p->x1198; }
void func_00138658(Obj *o) {
 float rate, limited, clamped; int v;
 if (func_00138560(o)) {
  rate=0.5f;
  if (o->x158->x7BB1 && o->x494 && o->x158->x6D28<=0) rate=1.0f;
  else if (o->x4A4<300) rate=1.0f-(float)o->x4A4*0.0016666667f;
  v=right(o->x158);
  if(v) o->x488+=rate*0.03f;
  else {
   v=left(o->x158);
   if(v) o->x488-=rate*0.03f;
   else o->x488*=0.95f;
  }
  __asm__ volatile("min.s %0,%1,%2" : "=&f"(limited) : "f"(o->x488), "f"(1.0f));
  o->x488=limited;
  __asm__ volatile("max.s %0,%1,%2" : "=&f"(clamped) : "f"(limited), "f"(-1.0f) : "memory");
  o->x488=clamped;
 } else func_00138808(o);
  v=right(o->x158);
 if(!v) { v=left(o->x158); if(!v) o->x498=1.0f; }
}
