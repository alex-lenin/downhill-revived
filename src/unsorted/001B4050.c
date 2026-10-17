/* UNIT 0x001B4050 0x001B4080 */
extern char D_004A9E40[];
extern void func_001B20B0(void *obj);

void func_001B4050(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        func_001B20B0(D_004A9E40);
    }
}
