/* UNIT 0x0017C830 0x0017C8A8 */
/* Walks a sequence of tagged blocks: byte 2 = element count, byte 3 = tag. */
unsigned char *func_0017C830(void *arg0, unsigned char *p)
{
    unsigned char *block;
    int count;
    int size;

    block = p + 8;
    if (block[3] == 0x60) {
        block = p + 0x10;
    }
    count = block[2];
    block += 4;
    block += count * 12;
    block += 0x18;
    count = block[2];
    block += count / 4 * 4 + 4;
    if (block[3] == 0x6F) {
        size = ((unsigned int)count >> 1) * 4;
    } else {
        size = count * 4;
    }
    block += size + 4;
    return block + 4;
}
