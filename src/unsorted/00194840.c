/* UNIT 0x00194840 0x00194898 */
typedef struct Entry {
    char pad0[0x32];
    signed char x32;
    char pad33[0x44 - 0x33];
    int x44;
} Entry;

extern int D_004B26AC[];
extern void func_001952A0(Entry *);

void func_00194840(int obj)
{
    int a, b, c;
    int i;

    i = D_004B26AC[0] - 4;
    a = i * 0x50;
    a += obj;
    a += 0x11A88;
    b = i * 0x50;
    b += obj;
    b += 0x11A88;
    c = i * 0x50;
    c += 0x11A88;
    ((Entry *)a)->x32 = 0;
    ((Entry *)b)->x44 = 0;
    func_001952A0((Entry *)(obj + c));
}
