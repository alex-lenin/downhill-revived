/* UNIT 0x00162D18 0x00162D80 .rodata=0x28A5D0:0x28A610 */
float func_00162D18(int type)
{
    switch (type) {
    case 11:
    case 25:
        return 1.0f;
    case 22:
    case 24:
        return 0.75f;
    case 26:
        return 1.25f;
    default:
        return 0.0f;
    }
}
