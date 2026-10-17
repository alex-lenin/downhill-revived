/* UNIT 0x00172460 0x00172488 */
extern char D_00285780[];
extern char *D_003AD920[];

void func_00172460(int enable, int id)
{
    if (id == 0xFFFF && enable != 0) {
        D_003AD920[0] = D_00285780;
    }
}
