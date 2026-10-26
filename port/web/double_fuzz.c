#include <stdint.h>
#ifndef __wasm__
#include <stdio.h>
#include <io.h>
#endif
double web_dadd(double,double),web_dsub(double,double),web_dmul(double,double),web_ddiv(double,double),web_du64(uint64_t),web_di64(int64_t);
double web_dsqrt(double);
static uint32_t next(uint32_t *s){uint32_t x=*s;x^=x<<13;x^=x>>17;x^=x<<5;return *s=x;}
/* Ops 7-9 (add, sub, mul) take the EE FPU's operands (ps2float.h: promoted floats), built from the
   bits so that both sides see the same values: a normal float's exponent and 23 significand bits,
   the second operand's exponent within 30 of the first's half the time (the exact fast paths and
   their edges); a non-normal float exponent draws a random binary64 instead. */
static uint64_t floatlike(uint32_t r,uint64_t other,int near){
    unsigned e=(r>>23)&255;
    if(near){int at=(int)(other>>52&2047)-896+(int)(r%61)-30;e=at<1?1:at>254?254:(unsigned)at;}
    if(!e||e==255)return ((uint64_t)r<<32)|(r*2654435761u);
    return ((uint64_t)(r&0x80000000u)<<32)|((uint64_t)(e+896)<<52)|((uint64_t)(r&0x7fffff)<<29);
}
static void pair(unsigned op,uint32_t *seed,uint64_t *x,uint64_t *y){
    *x=(uint64_t)next(seed)<<32;*x|=next(seed);
    *y=(uint64_t)next(seed)<<32;*y|=next(seed);
    if(op>=7){*x=floatlike((uint32_t)(*x>>32),0,0);*y=floatlike((uint32_t)(*y>>32),*x,(*y&1)!=0);}
}
static uint64_t check(unsigned op,uint64_t x,uint64_t y){
    double a,b,r;__builtin_memcpy(&a,&x,8);__builtin_memcpy(&b,&y,8);
#ifdef __wasm__
    switch(op){case 0:case 7:r=web_dadd(a,b);break;case 1:case 8:r=web_dsub(a,b);break;
    case 2:case 9:r=web_dmul(a,b);break;case 3:r=web_ddiv(a,b);break;
    case 4:r=web_di64(x);break;case 5:r=web_du64(x);break;default:r=web_dsqrt(a);break;}
#else
    switch(op){
    case 0:case 7:__asm__ volatile("addsd %1,%0":"+x"(a):"x"(b));r=a;break;
    case 1:case 8:__asm__ volatile("subsd %1,%0":"+x"(a):"x"(b));r=a;break;
    case 2:case 9:__asm__ volatile("mulsd %1,%0":"+x"(a):"x"(b));r=a;break;
    case 3:__asm__ volatile("divsd %1,%0":"+x"(a):"x"(b));r=a;break;
    case 4:__asm__ volatile("cvtsi2sd %1,%0":"=x"(r):"r"((int64_t)x));break;
    case 5:r=(double)x;break;default:__asm__ volatile("sqrtsd %1,%0":"=x"(r):"x"(a));break;
    }
#endif
    uint64_t u;__builtin_memcpy(&u,&r,8);return u;
}
uint32_t web_batch(unsigned op,uint32_t seed,unsigned n){
    uint64_t *out=(void *)0x01000000;*(volatile uint32_t *)0x07800000=0xffc0;
    for(unsigned k=0;k<n;k++){uint64_t x,y;pair(op,&seed,&x,&y);out[k]=check(op,x,y);}
    return seed;
}
#ifndef __wasm__
int main(void){
    _setmode(_fileno(stdout),0x8000);uint32_t mode=0xffc0;__asm__ volatile("ldmxcsr %0"::"m"(mode));
    static uint64_t out[65536];
    for(unsigned op=0;op<10;op++){
        uint32_t seed=0x19770321;
        for(unsigned i=0;i<1000000;){unsigned n=1000000-i;if(n>65536)n=65536;
            for(unsigned k=0;k<n;k++){uint64_t x,y;pair(op,&seed,&x,&y);out[k]=check(op,x,y);}
            if(fwrite(out,8,n,stdout)!=n)return 2;i+=n;
        }
    }return 0;
}
#endif
