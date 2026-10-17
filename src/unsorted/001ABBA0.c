/* UNIT 0x001ABBA0 0x001ABBC8 */
extern char D_00287E90[];
extern void *D_004A7A6C[];

void func_001ABBA0(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        D_004A7A6C[0] = D_00287E90;
    }
}
