/* UNIT 0x0020E480 0x0020E4C8 */
extern unsigned char D_004F2980[];

unsigned int func_0020E480(int offset)
{
    return D_004F2980[offset] + (D_004F2980[offset + 1] << 8) + (D_004F2980[offset + 2] << 16) + (D_004F2980[offset + 3] << 24);
}
