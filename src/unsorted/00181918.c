/* UNIT 0x00181918 0x00181940 */
extern char D_00285780[];
extern void *D_003BD5F0[];

void func_00181918(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_003BD5F0[0] = D_00285780;
    }
}
