/* Same seeded corpus in real wasm and native SSE; no game data. */
#include <stdint.h>
#ifndef __wasm__
#include <stdio.h>
#include <io.h>
#endif
float web_add(float,float),web_sub(float,float),web_mul(float,float),web_div(float,float),web_sqrt(float),web_demote(double),web_i32(int32_t),web_u32(uint32_t);
float web_i64(int64_t),web_u64(uint64_t);
static uint32_t next(uint32_t *s){uint32_t x=*s;x^=x<<13;x^=x>>17;x^=x<<5;return *s=x;}
static uint32_t check(unsigned op,uint32_t x,uint32_t y){
    float a,b,r;double d;uint64_t wide=(uint64_t)x<<32|y;
    __builtin_memcpy(&a,&x,4);__builtin_memcpy(&b,&y,4);__builtin_memcpy(&d,&wide,8);
#ifdef __wasm__
    switch(op){
    case 0:r=web_add(a,b);break;case 1:r=web_sub(a,b);break;
    case 2:r=web_mul(a,b);break;case 3:r=web_div(a,b);break;
    case 4:r=web_sqrt(a);break;case 5:r=web_demote(d);break;
    case 6:r=web_i32(x);break;case 7:r=web_u32(x);break;
    case 8:r=web_i64(wide);break;default:r=web_u64(wide);break;
    }
#else
    switch(op){
    case 0:__asm__ volatile("addss %1,%0":"+x"(a):"x"(b));r=a;break;
    case 1:__asm__ volatile("subss %1,%0":"+x"(a):"x"(b));r=a;break;
    case 2:__asm__ volatile("mulss %1,%0":"+x"(a):"x"(b));r=a;break;
    case 3:__asm__ volatile("divss %1,%0":"+x"(a):"x"(b));r=a;break;
    case 4:__asm__ volatile("sqrtss %1,%0":"=x"(r):"x"(a));break;
    case 5:__asm__ volatile("cvtsd2ss %1,%0":"=x"(r):"x"(d));break;
    case 6:__asm__ volatile("cvtsi2ss %1,%0":"=x"(r):"r"((int32_t)x));break;
    case 7:{int64_t z=x;__asm__ volatile("cvtsi2ss %1,%0":"=x"(r):"r"(z));break;}
    case 8:__asm__ volatile("cvtsi2ss %1,%0":"=x"(r):"r"((int64_t)wide));break;
    default:r=(float)wide;break;
    }
#endif
    uint32_t u;__builtin_memcpy(&u,&r,4);return u;
}
uint32_t web_fuzz(unsigned op,unsigned n){
    uint32_t seed=0x19770321,hash=2166136261;
#ifdef __wasm__
    *(volatile uint32_t *)0x07800000=0xffc0;
#else
    uint32_t mode=0xffc0;__asm__ volatile("ldmxcsr %0"::"m"(mode));
#endif
    for(unsigned i=0;i<n;i++){uint32_t x=next(&seed),y=next(&seed);hash=(hash^check(op,x,y))*16777619;}
    return hash;
}
uint32_t web_case(unsigned op,uint32_t x,uint32_t y){return check(op,x,y);}
uint32_t web_batch(unsigned op,uint32_t seed,unsigned n){
    uint32_t *out=(uint32_t *)0x01000000;
    *(volatile uint32_t *)0x07800000=0xffc0;
    for(unsigned i=0;i<n;i++){uint32_t x=next(&seed),y=next(&seed);out[i]=check(op,x,y);}
    return seed;
}
#ifndef __wasm__
int main(int argc,char **argv){
    if(argc>1){
        _setmode(_fileno(stdout),0x8000); /* Windows CRT binary pipe, no CR/LF translation. */
        static uint32_t out[65536];uint32_t mode=0xffc0;
        __asm__ volatile("ldmxcsr %0"::"m"(mode));
        for(unsigned op=0;op<10;op++){
            uint32_t seed=0x19770321;
            for(unsigned i=0;i<20000000;){
                unsigned n=20000000-i;if(n>65536)n=65536;
                for(unsigned k=0;k<n;k++){uint32_t x=next(&seed),y=next(&seed);out[k]=check(op,x,y);}
                if(fwrite(out,4,n,stdout)!=n)return 2;i+=n;
            }
        }return 0;
    }
    for(unsigned op=0;op<10;op++)printf("%u %08x\n",op,web_fuzz(op,20000000));return 0;
}
#endif
