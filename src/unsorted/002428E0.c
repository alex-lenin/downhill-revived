/* UNIT 0x002428E0 0x00242960 */
#include "sif_dma.h"
/* SIF DMA transfer descriptor (sceSifDmaData layout). */
typedef SifDmaRequest DmaData;

extern void func_0025A960(int mode);
extern int func_0025AAA0(DmaData *data, int count);
extern int func_0025AA80(int id);

/* Copies size bytes from src to IOP address dst over SIF DMA and waits for completion. */
int func_002428E0(void *dst, void *src, int size)
{
    DmaData data;
    int id;

    if (size <= 0) {
        return 0;
    }
    data.src = src;
    data.dst = dst;
    data.size = size;
    data.attr = 0;
    func_0025A960(0);
    id = func_0025AAA0(&data, 1);
    while (func_0025AA80(id) >= 0) {
    }
    return size;
}
