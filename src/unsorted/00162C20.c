/* UNIT 0x00162C20 0x00162CA0 */
extern char D_00768098[];
extern int func_00162B50(void *, void *);
int func_00162C20(char *output) {
    int size = 8;
    int count = 31;
    char *item;
    *(int *)output = 5;
    *(int *)(output + 4) = 32;
    item = D_00768098;
    do {
        int n = func_00162B50(item, output + size);
        item += 32;
        count--;
        size += n;
    } while (count >= 0);
    return size;
}
