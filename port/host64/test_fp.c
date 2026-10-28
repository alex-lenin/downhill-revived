/* The host's float control word and cycle counter for a host test program (port/tools/testprog.py,
   HOST_FP), as host_win32.c / host_posix.c give them to the game: the VU runtime switches MXCSR to
   the VU's rounding and back through these. x64 only (host_posix.c has the arm64 FPCR form). */
#include <stdint.h>
#include <immintrin.h>

unsigned host_fp_csr(void) { return _mm_getcsr(); }
void host_fp_set_csr(unsigned csr) { _mm_setcsr(csr); }
uint64_t host_cycles(void) { return __rdtsc(); }
