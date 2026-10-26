#include <stdint.h>
#include <wasm_simd128.h>
void simd_copy(uint32_t d,uint32_t s){wasm_v128_store((void *)(uintptr_t)d,wasm_v128_load((void *)(uintptr_t)s));}
void simd_lane(uint32_t d,uint32_t s){v128_t v=wasm_i32x4_splat(0);v=wasm_v128_load32_lane((void *)(uintptr_t)s,v,2);wasm_v128_store((void *)(uintptr_t)d,v);}
uint64_t leaf(double a,double b){
    /* Deliberately a leaf before postlink: clang need not publish its frame SP. */
    volatile uint64_t canary[16];
    for(unsigned i=0;i<16;i++)canary[i]=0x123456789abcdef0ull+i;
    volatile double result=a*b;
    uint64_t hash=0;for(unsigned i=0;i<16;i++)hash=hash*33+canary[i];
    return hash+(uint64_t)result;
}
uint32_t load(uint32_t a){return *(volatile uint32_t *)(uintptr_t)a;}
void store(uint32_t a,uint32_t v){*(volatile uint32_t *)(uintptr_t)a=v;}
uint32_t offset(uint32_t a){return ((volatile uint32_t *)(uintptr_t)a)[4];}
uint32_t load_based(uint32_t i){return ((volatile uint32_t *)(uintptr_t)0x40001000)[i];}
void store_based(uint32_t i,uint32_t v){((volatile uint32_t *)(uintptr_t)0x40001000)[i]=v;}
void copy(uint32_t d,uint32_t s,uint32_t n){__builtin_memcpy((void *)(uintptr_t)d,(void *)(uintptr_t)s,n);}
void fill(uint32_t d,uint32_t v,uint32_t n){__builtin_memset((void *)(uintptr_t)d,v,n);}
uint32_t bench(uint32_t address,uint32_t n){
    volatile uint32_t *p=(void *)(uintptr_t)address;uint32_t sum=0;
    for(uint32_t i=0;i<n;i++){sum+=p[i&255];p[i&255]=sum;}return sum;
}
