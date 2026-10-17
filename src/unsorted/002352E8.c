/* UNIT 0x002352E8 0x002353D8 */
/* Word heap: each block is [state (-1 = free, 0 = used), size in words, data...]. */
typedef struct Heap {
    unsigned int *base;
    unsigned int size;
} Heap;

extern void func_002353D8();

int func_002352E8(Heap *heap, unsigned int *blk)
{
    unsigned int *prev = heap->base;
    unsigned int *next;

    blk -= 2;
    if (blk < prev || blk[0] != 0) {
        return 0;
    }
    blk[0] = 0xFFFFFFFF;
    next = blk + blk[1] + 2;
    if (next < heap->base + heap->size && next[0] == 0xFFFFFFFF) {
        blk[1] += next[1] + 2;
    }
    while (prev + prev[1] + 2 < blk) {
        prev += prev[1] + 2;
    }
    if (prev < blk && prev[0] == 0xFFFFFFFF) {
        prev[1] += blk[1] + 2;
    }
    func_002353D8();
    return 1;
}
