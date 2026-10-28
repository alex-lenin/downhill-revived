/* Native x86 host services for standalone VU regression executables. */
#include <stdint.h>
#include <xmmintrin.h>
#include <intrin.h>

unsigned host_fp_csr(void) { return _mm_getcsr(); }
void host_fp_set_csr(unsigned control) { _mm_setcsr(control); }
uint64_t host_cycles(void) { return __rdtsc(); }
