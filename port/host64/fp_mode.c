#include "core_host.h"
#if defined(__x86_64__) || defined(_M_X64)
#include <xmmintrin.h>
void host_fp_mode(int mode) {
    /* EE entry preserves sticky status like host_win32.c. IEEE entry clears
       status like graphics_queue.c; VU callers restore their saved raw mode. */
    _mm_setcsr(mode==HOST_FP_EE ? (_mm_getcsr() & 63u) | 0xFFC0u : 0x1F80u);
}
#elif defined(__aarch64__)
/* Untested: FPCR.RMode=11 (RZ), FZ=1, exception enables clear.
   Arm tininess/NaN details still require device state-trace validation. */
void host_fp_mode(int mode) {
    uint64_t fpcr=mode==HOST_FP_EE ? (3ull<<22)|(1ull<<24) : 0;
    __asm__ volatile("msr fpcr, %0\n\tisb" :: "r"(fpcr) : "memory");
    if (mode!=HOST_FP_EE) __asm__ volatile("msr fpsr, xzr" ::: "memory");
}
#else
#error Implement host_fp_mode for this architecture
#endif
