/* UNIT 0x001BB750 0x001BB780 */
extern char D_004B25A0[];
extern void func_001B8C80(void *obj);

void func_001BB750(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        func_001B8C80(D_004B25A0);
    }
}
