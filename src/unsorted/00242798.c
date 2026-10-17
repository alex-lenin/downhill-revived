/* UNIT 0x00242798 0x002428E0 */
#include "sif_dma.h"
int func_00242798(char *out1,int capacity1,char *out2,int capacity2,
                  char *input1,int size1,char *input2,int size2)
{
    int end,excess,gap;
    end = size1 + size2;
    if (capacity1 + capacity2 < end) {
        excess = end - (capacity1 + capacity2);
        if (excess >= size2) {
            size1 -= excess - size2;
            size2 = 0;
        } else size2 -= excess;
    }
    gap = capacity1 - size1;
    if (size1 >= capacity1) {
        CopySifDmaDiscard(out1,input1,capacity1);
        CopySifDmaDiscard(out2,input1+capacity1,size1-capacity1);
        CopySifDmaDiscard(out2+size1-capacity1,input2,size2);
    } else if (size2 >= gap) {
        CopySifDmaDiscard(out1,input1,size1);
        CopySifDmaDiscard(out1+size1,input2,gap);
        CopySifDmaDiscard(out2,input2+capacity1-size1,size2-gap);
    } else {
        CopySifDmaDiscard(out1,input1,size1);
        CopySifDmaDiscard(out1+size1,input2,size2);
    }
    return size1+size2;
}
