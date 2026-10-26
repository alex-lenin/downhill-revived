/* Host address translation only: guest pointer VALUES are never rewritten.
   prepare_wasm2c.py applies this after wasm optimization, before native /O2. */
#ifndef DHD_HOST64_MEMORY_ACCESS_H
#define DHD_HOST64_MEMORY_ACCESS_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "wasm-rt.h"
#include "memory_layout.h"
#if WASM_RT_USE_SEGUE || WABT_BIG_ENDIAN
#error Host64 translation requires little-endian linear memory and Segue disabled
#endif
/* This is an allocator fact, not permission to ignore out-of-bounds accesses.
   Exposing it lets LLVM eliminate checks only for provably in-range addresses. */
static inline uint64_t host64_memory_size(uint64_t size) {
    __builtin_assume(size >= HOST64_MIN_MEMORY_SIZE);
    return size;
}
static inline int host64_alias_crossing(uint64_t address,uint64_t size) {
    uint64_t offset=address&0x1ffffffull, window=address&~0x1ffffffull;
    if (size>0x2000000ull-offset) return 1;
    /* Low RAM aliases start 4 MiB into these windows. */
    return (window==0 || window==0x20000000 || window==0x30000000) &&
        offset<0x400000 && size>0x400000-offset;
}
/* 32-MiB windows (address >> 25) that hold a RAM alias or a low view, where the code below may
   fold or treat a span as crossing: 0x00000000, 0x20000000, 0x28000000, 0x30000000, 0x38000000. */
#define HOST64_ALIAS_WINDOWS 0x11110001u
static inline uint8_t *host64_memory_address(uint8_t *base, uint64_t address,
                                             uint64_t size) {
    /* Fast path, the same result as the full test below: a span inside one 32-MiB window that
       is none of those five is neither folded nor crossing, so its host address is base +
       address. This covers canonical RAM (0x08000000) and the module's own data. A zero size
       (address - 1 in another window) and every other span take the full test. */
    uint64_t window = address >> 25;
    if (__builtin_expect(window == (address + size - 1) >> 25 &&
                         (window >= 32 || !((HOST64_ALIAS_WINDOWS >> window) & 1)), 1))
        return base + address;
    /* A span crossing a 32 MiB boundary may leave/enter an alias window.
       Preserve its exact virtual-byte sequence using the OS views. Such rare
       spans retain the original full barrier; never fold only their first byte.
       Bounds checks remain in wasm2c, against the ORIGINAL guest address. */
    if (host64_alias_crossing(address,size)) {
        uint8_t *p = base + address;
#if defined(__clang__) || defined(__GNUC__)
        __asm__ volatile("" : "+r"(p) : : "memory");
        return p;
#else
#error Host64 boundary spans require the compiler memory/alias barrier
#endif
    }
    /* Exactly [0x28000000,0x2a000000) and [0x38000000,0x3a000000).
       Keep the high 32 bits in the test: wasm effective addresses use u64. */
    if ((address & ~0x11ffffffull) == 0x28000000ull)
        address &= ~0x30000000ull;
    if ((address&0x1ffffffull)>=0x400000 &&
        (address<0x02000000 || (address>=0x20400000 && address<0x22000000) ||
         (address>=0x30400000 && address<0x32000000)))
        address=0x08000000+(address&0x1ffffffull);
    return base + address;
}
/* wasm memory.copy promises source-snapshot semantics. A crossing range may
   physically overlap a folded range even when their OS pointers look disjoint.
   Keep the usual memmove fast path; snapshot the rare crossing case. */
static inline void host64_memory_copy(uint8_t *dest, uint64_t d,
                                      const uint8_t *src, uint64_t s,
                                      uint64_t n) {
    if (host64_alias_crossing(d,n) || host64_alias_crossing(s,n)) {
        uint8_t *temporary = (uint8_t *)malloc((size_t)n);
        if (!temporary) wasm_rt_trap(WASM_RT_TRAP_EXHAUSTION);
        memcpy(temporary,host64_memory_address((uint8_t *)src,s,n),(size_t)n);
        memcpy(host64_memory_address(dest,d,n),temporary,(size_t)n);
        free(temporary);
    } else {
        memmove(host64_memory_address(dest,d,n),
                host64_memory_address((uint8_t *)src,s,n),(size_t)n);
    }
}
#endif
