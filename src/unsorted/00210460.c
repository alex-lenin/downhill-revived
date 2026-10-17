/* UNIT 0x00210460 0x00210578 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct {
    unsigned int *tags[4];
    unsigned char pkts[4][0x320];
} DmaChain;

extern void func_0025A960(int);

void func_00210460(DmaChain *chain, int start, int count)
{
    int i;
    unsigned int *tag;

    count--;
    for (i = start; i < count; i++) {
        tag = chain->tags[i];
        *(u128 *)tag = 0;
        tag[0] = 0x20000000;
        tag[1] = (unsigned int)chain->pkts[i + 1] & 0xFFFFFFF;
    }
    tag = chain->tags[i];
    tag[0] = 0x20000000;
    tag[1] = (unsigned int)((char *)chain + 0xCA0) & 0xFFFFFFF;
    func_0025A960(0);
    *(volatile unsigned int *)0x1000A020 = 0;
    *(volatile unsigned int *)0x1000E010 = 4;
    *(volatile unsigned int *)0x1000A030 = (unsigned int)chain->pkts[start] & 0xFFFFFFF;
    *(volatile unsigned int *)0x1000A000 = 0x145;
}
