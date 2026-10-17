/* UNIT 0x00193F38 0x00193F68 */
extern int D_004B2828[];

int func_00193F38(char *obj)
{
    if (D_004B2828[0] >= 3) {
        return 0x20001C2;
    }
    return *(int *)(obj + 0x12114);
}
