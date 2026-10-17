/* UNIT 0x00202738 0x00202760 */
extern int func_00201E90(int cmd, int size, void *data);

int func_00202738(int arg)
{
    return func_00201E90(0x4F, 4, &arg);
}
