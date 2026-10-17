/* UNIT 0x00235218 0x002352E8 */
/* Word heap: each block is [state (-1 = free, 0 = used), size in words, data...]. */
typedef struct Heap {
    unsigned int *base;
    unsigned int size;
} Heap;

extern void func_002353D8(Heap *heap);

void *func_00235218(Heap *heap, unsigned int n)
{
    unsigned int *blk;
    unsigned int *rest;

    for (blk = heap->base; blk < heap->base + heap->size; blk += blk[1] + 2) {
        if (blk[0] == 0xFFFFFFFF && blk[1] >= n) {
            if (blk[1] > n + 2) {
                rest = blk + n + 2;
                rest[0] = 0xFFFFFFFF;
                rest[1] = blk[1] - n - 2;
                blk[1] = n;
            }
            blk[0] = 0;
            func_002353D8(heap);
            return blk + 2;
        }
    }
    func_002353D8(heap);
    return 0;
}
