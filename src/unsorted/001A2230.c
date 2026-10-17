/* UNIT 0x001A2230 0x001A2260 */
extern char D_0043F350[];
extern void func_001A06F8(void *p);

void func_001A2230(int enable, int id)
{
    if (id == 0xFFFF && enable) {
        func_001A06F8(D_0043F350);
    }
}
