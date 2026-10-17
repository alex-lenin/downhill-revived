/* UNIT 0x0012F8D8 0x0012FA70 */
/* func_0012F8D8: rider state flag update.
   If either selector (x13D5 ? x1250 : x1188, x13D6 ? x1254 : x118C) is set,
   recompute x7A69: when set and x2E98 > 0, keep it unless func_00138560 says
   clear and the x7AC0/threshold test fails; when clear, set from the
   x2E98-vs-x2EB4 comparison unless func_00138560 says clear. Mirror x7A69
   into x2EC0 on change, then tick one of the x7964/x7968 counters. */
extern int func_00138560(void *);
extern int func_00238F60();
extern float fabsf(float);
extern float D_0039E7F8[];
#define F(o,n) (*(float *)((o)+(n)))
#define I(o,n) (*(int *)((o)+(n)))
#define B(o,n) (*(unsigned char *)((o)+(n)))
#define H(o,n) (*(unsigned short *)((o)+(n)))
static inline int first(char *o) { if(B(o,0x13D5)) return I(o,0x1250); return I(o,0x1188); }
static inline int second(char *o) { if(B(o,0x13D6)) return I(o,0x1254); return I(o,0x118C); }
static inline int positive(float t, float zero) { return t>zero; }
static inline unsigned char lower(char *p) { return F(p,4)>F(p,0x20); }
static inline void setState(char *p, int v) { B(p,0x2EC0)=(unsigned char)v; }
static inline float fmax(float a, float b) { float r; __asm__ volatile("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b)); return r; }
void func_0012F8D8(char *o) {
 int state=0; float dummy, v, limit, magnitude, complement, threshold, t, zero;
 if(first(o) || second(o)) {
  if(B(o,0x7A69)) {
   t=F(o,0x2E98);
   zero=0.0f;
   if((unsigned char)positive(t, zero)) {
    if(!(func_00138560(o+0x40C0)&255)) state=1;
    else {
     /* Zero-byte asm labels (cf. src/unsorted/00176ED8.c): Lfdum/LfuseD/
        LfuseV are scheduling barriers only -- they emit no bytes. They pin
        the 1.0f/0.7f constant loads below abs.s (retail order; otherwise the
        scheduler hoists one into the lw load-delay slot) and shape FP
        allocation so the load/abs use $f1/$f2. */
     __asm__ volatile ("Lfdum:" : "=f" (dummy));
     v=F(*(char **)(o+0x792C),0x58);
     __asm__ volatile ("LfuseD:" : : "f" (dummy));
     magnitude=fabsf(v);
     __asm__ volatile ("LfuseV:" : : "f" (v));
     complement=1.0f-magnitude;
     limit=fmax(complement, 0.7f);
     threshold=D_0039E7F8[0]/limit;
     if(F(o,0x7AC0)!=zero && F(o,0x7AC0)<threshold) state=1;
    }
   }
  } else if(!(func_00138560(o+0x40C0)&255)) state=lower(o+0x2E94);
 }
 if(B(o,0x7A69)!=state) { B(o,0x7A69)=state; setState(o,B(o,0x7A69)); }
 if(B(o,0x7A69)) { int dt=func_00238F60(); H(o,0x7968)=0; H(o,0x7964)+=dt; }
 else { H(o,0x7964)=0; H(o,0x7968)+=func_00238F60(); }
}
