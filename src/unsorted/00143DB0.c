/* UNIT 0x00143DB0 0x00143E18 */
/* Clear two status words in each of 10 sub-entries of the global table. */
typedef struct {
    char pad[0x70];
    int x70;
    char pad74[0xC];
    int x80;
    char pad84[0xEC];
} Sub;

typedef struct {
    char pad[0x70];
    Sub subs[10];
    char tail[0xB0];
} Item;

extern char D_0036B870[];

void func_00143DB0(int arg0, int arg1)
{
    int *q;
    Item *item;
    int i;
    int j;
    Item *next;
    int remaining;

    if (arg1 != 0xFFFF)
        return;
    if (arg0 == 0)
        return;
    item = (Item *)D_0036B870;
    item = (Item *)((char *)item + 0x10);
    i = 0;
    do {
        next = item + 1;
        remaining = i - 1;
        q = (int *)((char *)item + 0xF0);
        j = 9;
        do {
            q[-4] = 0;
            q[0] = 0;
            q += 0x5C;
        } while (--j != -1);
        i = remaining;
        item = next;
    } while (i != -1);
}
