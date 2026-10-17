/* UNIT 0x001E6EB0 0x001E6EF8 */
struct Row {
    int x0;
    int values[8];
};

extern struct Row D_00272448[];

void func_001E6EB0(int index, int *dest)
{
    int i;

    for (i = 0; i < 8; i++) {
        dest[i] = D_00272448[index].values[i];
    }
}
