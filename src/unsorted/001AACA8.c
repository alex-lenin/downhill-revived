/* UNIT 0x001AACA8 0x001AAD00 */
struct FreeList { unsigned char x[8]; };
extern struct FreeList D_0029E4D8;
extern unsigned char D_0029E4E0;
extern unsigned char D_004A5990[];

void func_001AACA8(char *arg0)
{
    unsigned char *a;
    unsigned char *b;
    unsigned char t;

    D_0029E4E0 = D_0029E4E0 - 1;
    a = (unsigned char *)((int)D_0029E4D8.x + ((((int)arg0 - (int)D_004A5990) * -858993459) >> 6));
    b = (unsigned char *)((int)D_0029E4D8.x + D_0029E4E0);
    t = *b;
    *b = *a;
    *a = t;
}
