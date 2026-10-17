/* UNIT 0x00183218 0x00183238 */
extern int D_003C6408[];

void func_00183218(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_003C6408[0] = 0;
    }
}
