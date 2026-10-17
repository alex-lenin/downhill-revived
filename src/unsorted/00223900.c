/* UNIT 0x00223900 0x00223930 */
extern char D_00663140[];
extern void func_00242FB0(void *p);

void func_00223900(int enable, int id)
{
    if (id == 0xFFFF) {
        if (enable != 0) {
            func_00242FB0(D_00663140);
        }
    }
}
