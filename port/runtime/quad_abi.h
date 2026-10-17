/* EE scalar quadwords occupy one whole GPR. GCC mode(TI) keeps the native
   16-byte scalar calling convention on i386, where spelling __int128 directly
   is unsupported. memcpy preserves all bits without an alignment/alias cast. */
#ifndef DHD_QUAD_ABI_H
#define DHD_QUAD_ABI_H
#include "ps2asm.h"

typedef unsigned int ps2_u128 __attribute__((mode(TI)));
typedef char ps2_u128_size_check[sizeof(ps2_u128) == 16 ? 1 : -1];

static inline ps2_u128 ps2_quad_scalar(ps2_q128 value)
{
    ps2_u128 scalar;
    __builtin_memcpy(&scalar, &value, 16);
    return scalar;
}

static inline ps2_q128 ps2_scalar_quad(ps2_u128 scalar)
{
    ps2_q128 value;
    __builtin_memcpy(&value, &scalar, 16);
    return value;
}
#endif
