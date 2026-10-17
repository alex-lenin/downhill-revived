/* UNIT 0x001992F0 0x00199318 */
extern char D_00287958[];
extern void *D_0043A7AC[];

void func_001992F0(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_0043A7AC[0] = D_00287958;
    }
}
