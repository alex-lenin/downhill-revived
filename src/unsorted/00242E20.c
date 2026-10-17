/* UNIT 0x00242E20 0x00242E40 */
extern void **D_0029E49C;

void *func_00242E20(void)
{
    void **node = D_0029E49C;
    D_0029E49C = (void **)*node;
    *node = 0;
    return node;
}
