/* UNIT 0x00201980 0x002019A8 */
extern int func_00201E90(int cmd, int size, void *data);

int func_00201980(int arg)
{
    return func_00201E90(0xA, 4, &arg);
}
