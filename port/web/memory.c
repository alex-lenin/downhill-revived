/* Web RAM aliases. Pointer values are preserved; only effective accesses fold. */
#include <stdint.h>
static uint32_t fold(uint32_t a) {
    uint32_t low=a&0x1fffffff;
    if ((a&0xe0000000)==0 || (a&0xe0000000)==0x20000000) {
        if (low>=0x08000000 && low<0x0a000000) return low;
        if (low>=0x00400000 && low<0x02000000) return low+0x08000000;
        /* 0x30000000 aliases use a different upper mask. */
    }
    if (a>=0x38000000 && a<0x3a000000) return a-0x30000000;
    if (a>=0x30400000 && a<0x32000000) return a-0x28000000;
    return a;
}
static uint32_t checked(uint32_t a,uint32_t off,uint32_t n) {
    uint64_t e=(uint64_t)a+off;
    uint64_t size=(uint64_t)__builtin_wasm_memory_size(0)*65536;
    if (e+n>size) __builtin_trap();
    return (uint32_t)e;
}
/* Independent debug certificate. Scalar/SIMD accesses are at most 16 bytes;
   no alias window is shorter than 28 MiB. With wrapping excluded, unchanged
   endpoints therefore prove every byte unchanged. Trap for wider requests:
   endpoint equality alone is NOT a valid bulk-memory certificate. */
void web_assert_unfolded(uint32_t a,uint32_t off,uint32_t n) {
    a=checked(a,off,n);
    if(!n || n>16 || fold(a)!=a || fold(a+n-1)!=a+n-1)__builtin_trap();
}
/* The same certificate for postlink's constant folds (low view, 0x38000000 window): every byte
   folds by exactly delta. Equal deltas at both endpoints of a span of at most 16 bytes put it
   inside one alias window (the windows are 4 MiB or more apart and 28 MiB or longer). */
void web_assert_folded(uint32_t a,uint32_t off,uint32_t n,uint32_t delta) {
    a=checked(a,off,n);
    if(!n || n>16 || delta==0 || fold(a)!=a+delta || fold(a+n-1)!=a+n-1+delta)__builtin_trap();
}
uint32_t web_prepare(uint32_t a,uint32_t off,uint32_t n) {
    a=checked(a,off,n);
    uint32_t p=fold(a);
    if (fold(a+n-1)==p+n-1) return p;
    volatile uint8_t *tmp=(void *)0x07810000;
    for (uint32_t i=0;i<n;i++) tmp[i]=*(volatile uint8_t *)(uintptr_t)fold(a+i);
    return 0x07810000;
}
void web_finish(uint32_t a,uint32_t off,uint32_t n) {
    a=checked(a,off,n);
    uint32_t p=fold(a);
    if (fold(a+n-1)==p+n-1) return;
    volatile uint8_t *tmp=(void *)0x07810000;
    for (uint32_t i=0;i<n;i++) *(volatile uint8_t *)(uintptr_t)fold(a+i)=tmp[i];
}
static int contiguous(uint32_t a,uint32_t n) {
    /* Equal folded endpoints alone miss a span containing an entire alias. */
#define EDGE(e) if(a<(e) && n>(e)-a)return 0
    EDGE(0x00400000u);EDGE(0x02000000u);
    EDGE(0x20400000u);EDGE(0x22000000u);
    EDGE(0x28000000u);EDGE(0x2a000000u);
    EDGE(0x30400000u);EDGE(0x32000000u);
    EDGE(0x38000000u);EDGE(0x3a000000u);
#undef EDGE
    return 1;
}
void web_copy(uint32_t d,uint32_t s,uint32_t n) {
    checked(d,0,n); checked(s,0,n);
    if (!n) return;
    uint32_t fd=fold(d),fs=fold(s);
    if (contiguous(d,n) && contiguous(s,n)) {
        __builtin_memmove((void *)(uintptr_t)fd,(void *)(uintptr_t)fs,n);
    } else {
        /* Reserved web scratch, never guest RAM. Explicit bounded slow path. */
        if (n>0x02000000) __builtin_trap();
        volatile uint8_t *tmp=(void *)0x50000000;
        for (uint32_t i=0;i<n;i++) tmp[i]=*(volatile uint8_t *)(uintptr_t)fold(s+i);
        for (uint32_t i=0;i<n;i++) *(volatile uint8_t *)(uintptr_t)fold(d+i)=tmp[i];
    }
}
void web_fill(uint32_t d,uint32_t value,uint32_t n) {
    checked(d,0,n);
    if(!n)return;
    uint32_t fd=fold(d);
    if(contiguous(d,n)){__builtin_memset((void *)(uintptr_t)fd,value,n);return;}
    for (uint32_t i=0;i<n;i++) *(volatile uint8_t *)(uintptr_t)fold(d+i)=(uint8_t)value;
}
