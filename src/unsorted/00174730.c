/* UNIT 0x00174730 0x00174758 */
extern char D_00285780[];
extern char *D_003B0250[];

void func_00174730(int enable, int id)
{
    if (id == 0xFFFF && enable != 0) {
        D_003B0250[0] = D_00285780;
    }
}
