/* UNIT 0x001947B0 0x00194840 */
typedef struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x44 - 0x33];
    int x44;
} Entry;

extern int D_004B26AC[];
extern int func_001821E8(int, signed char);

int func_001947B0(int obj)
{
    int result;
    int n = D_004B26AC[0];
    int index = n - 4;
    int addr;

    addr = index * 0x50;
    addr += obj;
    addr += 0x11A88;
    result = func_001821E8(n, ++((Entry *)addr)->x32) == 0x21;
    /* reusing index for the address keeps gcc from CSEing the multiply across the call */
    index *= 0x50;
    index += obj;
    index += 0x11A88;
    ((Entry *)index)->x44 = 0;
    return result;
}
