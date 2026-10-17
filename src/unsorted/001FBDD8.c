/* UNIT 0x001FBDD8 0x001FC3A8 -sce29 */
/* Unsigned double-word division, specialized from the inline quotient/remainder helper. */
typedef unsigned int UWtype __attribute__((mode(SI)));
typedef int Wtype __attribute__((mode(SI)));
typedef unsigned int UDWtype __attribute__((mode(DI)));
typedef int DWtype __attribute__((mode(DI)));
typedef union { struct { Wtype low, high; } s; DWtype ll; } DWunion;
extern const unsigned char D_00297238[];
#define B (1L << 16)
#define lowpart(t) ((UWtype)(t) & (B - 1))
#define highpart(t) ((UWtype)(t) >> 16)
/* The generic width branch preserves the original inlined label alignment. */
#define count_leading_zeros(count,x) do { \
 UWtype xx = (x); UWtype a; \
 if (sizeof(UWtype)*8 <= 32) { \
 a = xx < ((UWtype)1 << 16) \
     ? (xx < ((UWtype)1 << 8) ? 0 : 8) \
     : (xx < ((UWtype)1 << 24) ? 16 : 24); \
 } else { \
  for (a=sizeof(UWtype)*8-8; a>0; a-=8) \
   if (((xx>>a)&255)!=0) break; \
 } \
 (count) = 32 - (D_00297238[xx >> a] + a); \
} while (0)
#define udiv_qrnnd(q,r,n1,n0,d) do { \
 UWtype __d1,__d0,__q1,__q0,__r1,__r0,__m; \
 __d1=highpart(d); __d0=lowpart(d); \
 __r1=(n1)%__d1; __q1=(n1)/__d1; __m=__q1*__d0; \
 __r1=__r1*B | highpart(n0); \
 if(__r1<__m) { __q1--, __r1+=(d); if(__r1>=(d)) if(__r1<__m) __q1--, __r1+=(d); } \
 __r1-=__m; \
 __r0=__r1%__d1; __q0=__r1/__d1; __m=__q0*__d0; \
 __r0=__r0*B | lowpart(n0); \
 if(__r0<__m) {__q0--, __r0+=(d); if(__r0>=(d)) if(__r0<__m) __q0--, __r0+=(d); } \
 __r0-=__m; \
 (q)=__q1*B | __q0; (r)=__r0; \
} while (0)
/* umul_ppmm as in gcc's longlong.h for MIPS (docs/findings/014). */
#define umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3" \
           : "=l" ((UWtype) (w0)), \
             "=h" ((UWtype) (w1)) \
           : "d" ((UWtype) (u)), \
             "d" ((UWtype) (v)))
#define sub_ddmmss(sh,sl,ah,al,bh,bl) do { \
 UWtype x = (al) - (bl); (sh) = (ah) - (bh) - (x > (al)); (sl) = x; \
} while (0)
static __inline__ UDWtype divide_with_remainder(UDWtype n, UDWtype d, UDWtype *rp)
{
 DWunion ww;
 DWunion nn,dd;
 DWunion rr;
 UWtype d0,d1,n0,n1,n2;
 UWtype q0,q1;
 UWtype b,bm;
 nn.ll=n; dd.ll=d;
 d0=dd.s.low; d1=dd.s.high;
 n0=nn.s.low; n1=nn.s.high;
 if(d1==0) {
  if(d0>n1) {
   count_leading_zeros(bm,d0);
   if(bm!=0) {d0=d0<<bm; n1=(n1<<bm)|(n0>>(32-bm)); n0=n0<<bm;}
   udiv_qrnnd(q0,n0,n1,n0,d0);
   q1=0;
  } else {
   if(d0==0) d0=1/d0;
   count_leading_zeros(bm,d0);
   if(bm==0) {n1-=d0; q1=1;}
   else {b=32-bm; d0=d0<<bm; n2=n1>>b; n1=(n1<<bm)|(n0>>b); n0=n0<<bm; udiv_qrnnd(q1,n1,n2,n1,d0);}
   udiv_qrnnd(q0,n0,n1,n0,d0);
  }
  if(rp!=0) {rr.s.low=n0>>bm; rr.s.high=0; *rp=rr.ll;}
 } else {
  if(d1>n1) {
   q0=0; q1=0;
   if(rp!=0) {rr.s.low=n0; rr.s.high=n1; *rp=rr.ll;}
  }
  else {
   count_leading_zeros(bm,d1);
   if(bm==0) {
    if(n1>d1 || n0>=d0) {q0=1; sub_ddmmss(n1,n0,n1,n0,d1,d0);} else q0=0;
    q1=0;
    if(rp!=0) {rr.s.low=n0; rr.s.high=n1; *rp=rr.ll;}
   }
   else {
    UWtype m1,m0;
    b=32-bm; d1=(d1<<bm)|(d0>>b); d0=d0<<bm;
    n2=n1>>b; n1=(n1<<bm)|(n0>>b); n0=n0<<bm;
    udiv_qrnnd(q0,n1,n2,n1,d1);
    umul_ppmm(m1,m0,q0,d0);
    if(m1>n1 || (m1==n1 && m0>n0)) {q0--; sub_ddmmss(m1,m0,m1,m0,d1,d0);}
    q1=0;
    if(rp!=0) {
     sub_ddmmss(n1,n0,n1,n0,m1,m0);
     rr.s.low=(n1<<b)|(n0>>bm); rr.s.high=n1>>bm; *rp=rr.ll;
    }
   }
  }
 }
 ww.s.low=q0; ww.s.high=q1; return ww.ll;
}
UDWtype func_001FBDD8(UDWtype n, UDWtype d)
{
 return divide_with_remainder(n,d,(UDWtype *)0);
}
