/* UNIT 0x001F8458 0x001F84A8 */
int func_001F8458(unsigned int type)
{
    int result = 0;

    if (type >= 5 && type != 17 && type != 15 && type != 6 && type != 7
        && type != 8 && type != 9) {
        result = type != 16;
    }
    return result;
}
