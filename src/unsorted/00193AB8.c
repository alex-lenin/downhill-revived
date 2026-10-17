/* UNIT 0x00193AB8 0x00193AE0 */
extern void *D_00412580[];
extern char D_002873B0[];

/* static initialization: sets the vtable of a global object */
void func_00193AB8(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize) {
        D_00412580[0] = D_002873B0;
    }
}
