/* UNIT 0x00195AA0 0x00195AF8 */
typedef struct Block {
    long data[27];
} Block;

Block *func_00195AA0(Block *dst, Block *src)
{
    *dst = *src;
    return dst;
}
