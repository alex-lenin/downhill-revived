/* UNIT 0x00242270 0x00242290 .sbss=0x29E468:0x29E470 */
int D_0029E468 __attribute__((section(".sbss"))) = 0;
extern int D_0029E48C;

void func_00242270(int enable, int id)
{
    if (id == 0xFFFF && enable != 0) {
        D_0029E468 = D_0029E48C;
    }
}
