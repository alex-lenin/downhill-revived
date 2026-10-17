/* UNIT 0x001223C0 0x001223E8 */
extern char D_00285780[];
extern char *D_00304220[];

void func_001223C0(int enable, int id)
{
    if (id == 0xFFFF && enable) {
        D_00304220[0] = D_00285780;
    }
}
