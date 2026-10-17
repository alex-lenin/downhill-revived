#ifndef DHD_SIF_DMA_H
#define DHD_SIF_DMA_H
typedef struct SifDmaRequest { void *src, *dst; int size, attr; } SifDmaRequest;
typedef char SifDmaRequest_size_check[sizeof(SifDmaRequest)==16 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int func_002428E0(void *dst, void *src, int size);
#if defined(__mips__)
void CopySifDmaDiscard(void *, void *, int) __asm__("func_002428E0");
#else
static inline void CopySifDmaDiscard(void *dst, void *src, int size) { func_002428E0(dst,src,size); }
#endif
#ifdef __cplusplus
}
#endif
#endif
