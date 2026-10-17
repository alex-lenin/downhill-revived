#ifndef DHD_MATRIX_PACKETS_H
#define DHD_MATRIX_PACKETS_H
#include "vector.h"
#ifdef __cplusplus
extern "C" {
#endif
void func_002239B0(Matrix4 *, const Matrix4 *, const Matrix4 *);
void func_00223A38(Matrix4 *, const Matrix4 *);
void func_00223A68(Matrix4 *);
#if defined(__mips__)
float *ComposeMatrixResultLegacy(void *, void *, void *) __asm__("func_002239B0");
int CopyMatrixAddressLegacy(int, const void *) __asm__("func_00223A38");
int IdentityMatrixAddressLegacy(int) __asm__("func_00223A68");
#else
/* The old result only feeds an unused asm operand; return the actual destination. */
static inline float *ComposeMatrixResultLegacy(void *out, void *a, void *b) { func_002239B0((Matrix4 *)out,(const Matrix4 *)a,(const Matrix4 *)b); return (float *)out; }
static inline void CopyMatrixAddressLegacy(int out, const void *in) { func_00223A38((Matrix4 *)out,(const Matrix4 *)in); }
static inline void IdentityMatrixAddressLegacy(int out) { func_00223A68((Matrix4 *)out); }
#endif
#ifdef __cplusplus
}
#endif
#endif
