/* UNIT 0x00219EC8 0x00219EE8 */
extern int D_0029E224;
extern int D_0029E228;

void func_00219EC8(unsigned int *p)
{
    D_0029E224 = (*p & 0x1FF) << 5;
    D_0029E228 = 0;
}
