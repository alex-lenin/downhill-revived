/* The native wasm2c module inherits MXCSR for raw f64 instructions too.
   This includes HLE scheduling and wasi-libc math internals. */
#include <stdint.h>
typedef __uint128_t u128;
static uint64_t bits(double x){uint64_t u;__builtin_memcpy(&u,&x,8);return u;}
static double value(uint64_t u){double x;__builtin_memcpy(&x,&u,8);return x;}
/* The FP control word: the core's at 0x07800000; the threaded build's graphics reactor has its own
   (build_graphics.py --threads builds these helpers with WEB_CSR_AT=web_queue.h's WEB_Q_REACTOR_CSR). */
#ifndef WEB_CSR_AT
#define WEB_CSR_AT 0x07800000u
#endif
static int ee(void){return (*(volatile uint32_t *)WEB_CSR_AT&0x6000)==0x6000;}
static double daz(double x){uint64_t u=bits(x);return value(u&0x7ff0000000000000ull?u:u&0x8000000000000000ull);}
static double ftz(double x){return daz(x);}
static double special(double a,double b){
    uint64_t x=bits(a),y=bits(b);
    if((x&0x7fffffffffffffffull)>0x7ff0000000000000ull)return value(x|0x8000000000000ull);
    if((y&0x7fffffffffffffffull)>0x7ff0000000000000ull)return value(y|0x8000000000000ull);
    return value(0xfff8000000000000ull);
}
static double sum(double a,double b){
    a=daz(a);b=daz(b);double s=a+b;
    if(__builtin_isnan(s))return special(a,b);
    if(__builtin_isinf(s)&&__builtin_isfinite(a)&&__builtin_isfinite(b))return value(bits(s)-1);
    double z=s-a,err=(a-(s-z))+(b-z);
    if((s>0&&err<0)||(s<0&&err>0))s=value(bits(s)-1);
    return ftz(s);
}
/* The EE FPU's binary64 operands (ps2float.h computes ADD, SUB and MUL in binary64 from promoted
   floats): at most 24 significant bits (the low 29 significand bits zero) and the exponent of a
   normal float. For two of them the binary64 product is exact (48 bits, a normal binary64), and so
   is the sum when the exponents are at most 28 apart (at most 53 bits, an exact zero or a normal
   binary64): rounding toward zero, DAZ and FTZ then change nothing. */
static int floatlike(uint64_t u){return !(u&0x1fffffffull)&&((unsigned)(u>>52&2047)-897u)<254u;}
static int exact_sum(uint64_t x,uint64_t y){
    int gap=(int)(x>>52&2047)-(int)(y>>52&2047);
    return floatlike(x)&&floatlike(y)&&gap<=28&&gap>=-28;
}
double web_dadd(double a,double b){
    if(!ee()||exact_sum(bits(a),bits(b)))return a+b;
    return sum(a,b);
}
double web_dsub(double a,double b){
    if(!ee()||exact_sum(bits(a),bits(b)))return a-b;
    if(__builtin_isnan(a)||__builtin_isnan(b))return special(a,b);
    return sum(a,value(bits(b)^0x8000000000000000ull));
}
static double pack(u128 mant,int e,uint64_t sign){
    uint64_t hi=(uint64_t)(mant>>64),lo=(uint64_t)mant;
    if(!hi&&!lo)return value(sign);
    int top=hi?127-__builtin_clzll(hi):63-__builtin_clzll(lo);
    int exp=e+top+1023;
    if(exp<=0)return value(sign);
    if(exp>=2047)return value(sign|0x7fefffffffffffffull);
    uint64_t q=top>52?(uint64_t)(mant>>(top-52)):(uint64_t)mant<<(52-top);
    return value(sign|((uint64_t)exp<<52)|(q&0xfffffffffffffull));
}
double web_dmul(double a,double b){
    if(!ee()||(floatlike(bits(a))&&floatlike(bits(b))))return a*b;
    a=daz(a);b=daz(b);
    uint64_t x=bits(a),y=bits(b),ax=x&0x7fffffffffffffffull,ay=y&0x7fffffffffffffffull;
    if(!ax||!ay||ax>=0x7ff0000000000000ull||ay>=0x7ff0000000000000ull){
        double d=a*b;return __builtin_isnan(d)?special(a,b):d;
    }
    uint64_t ma=(x&0xfffffffffffffull)|0x10000000000000ull,mb=(y&0xfffffffffffffull)|0x10000000000000ull;
    return pack((u128)ma*mb,(int)(x>>52&2047)+(int)(y>>52&2047)-2046-104,(x^y)&0x8000000000000000ull);
}
double web_ddiv(double a,double b){
    if(!ee())return a/b;a=daz(a);b=daz(b);
    uint64_t x=bits(a),y=bits(b),ax=x&0x7fffffffffffffffull,ay=y&0x7fffffffffffffffull;
    if(!ax||!ay||ax>=0x7ff0000000000000ull||ay>=0x7ff0000000000000ull){
        double d=a/b;return __builtin_isnan(d)?special(a,b):d;
    }
    /* RN is at most one representable result above the RZ magnitude. Decide
       the correction by an EXACT 106-bit product, never a rounded r*b.
       For normal finite operands/result the alignment is only 52 or 53 bits.
       This replaces 54 iterations of software 128-bit long division. */
    double r=a/b;uint64_t z=bits(r),az=z&0x7fffffffffffffffull;
    if(az>=0x7ff0000000000000ull)return value((z&0x8000000000000000ull)|0x7fefffffffffffffull);
    if(az<0x0010000000000000ull)return value(z&0x8000000000000000ull);
    uint64_t ma=(x&0xfffffffffffffull)|0x10000000000000ull,mb=(y&0xfffffffffffffull)|0x10000000000000ull;
    uint64_t mr=(z&0xfffffffffffffull)|0x10000000000000ull;
    int shift=(int)(x>>52&2047)-(int)(y>>52&2047)-(int)(z>>52&2047)+1075;
    if((u128)mr*mb>((u128)ma<<shift))r=value(z-1);
    return ftz(r);
}
double web_du64(uint64_t a){return ee()?pack(a,0,0):(double)a;}
double web_di64(int64_t a){return ee()?pack(a<0?0-(uint64_t)a:(uint64_t)a,0,a<0?0x8000000000000000ull:0):(double)a;}
double web_dsqrt(double a){
    if(!ee())return __builtin_sqrt(a);a=daz(a);
    if(__builtin_isnan(a))return special(a,a);
    if(a<0)return value(0xfff8000000000000ull);
    double r=__builtin_sqrt(a);
    if(a==0||__builtin_isinf(a))return r;
    uint64_t x=bits(a),y=bits(r);
    uint64_t ma=(x&0xfffffffffffffull)|0x10000000000000ull,mr=(y&0xfffffffffffffull)|0x10000000000000ull;
    int shift=(int)(x>>52&2047)-2*(int)(y>>52&2047)+1075;
    if((u128)mr*mr>((u128)ma<<shift))r=value(y-1);
    return r;
}
double web_promote(float a){
    uint32_t u;__builtin_memcpy(&u,&a,4);
    if(ee()&&!(u&0x7f800000))u&=0x80000000;
    __builtin_memcpy(&a,&u,4);return (double)a;
}
