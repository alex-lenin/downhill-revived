/* UNIT 0x00122258 0x00122360 */
int func_00122258(void *obj, int id)
{
    int flag = 0;

    if (id == 0x4B0) {
        flag = 0x4;
    } else if (id == 0x4B5) {
        flag = 0x8;
    } else if (id == 0x4B8) {
        flag = 0x10;
    } else if (id == 0x4BA) {
        flag = 0x20;
    } else if (id == 0x4BF || id == 0x4C1 || id == 0x4C3) {
        flag = 0x40;
    } else if (id == 0x4C6) {
        flag = 0x80;
    } else if (id == 0x4C9) {
        flag = 0x100;
    } else if (id == 0x4D0) {
        flag = 0x200;
    } else if (id == 0x4D3 || id == 0x5D1 || id == 0x5D3) {
        flag = 0x400;
    } else if (id == 0x4D5) {
        flag = 0x800;
    } else if (id == 0x4CB || id == 0x4E3 || id == 0x4E5) {
        flag = 0x1000;
    } else if (id == 0x5CD || id == 0x5CF) {
        flag = 0x2000;
    }
    return flag;
}
