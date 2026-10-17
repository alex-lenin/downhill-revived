/* UNIT 0x001DBC10 0x001DBC38 */
extern char D_00285780[];
extern void *D_004BD930[];

void func_001DBC10(void *obj, int id) {
    if (id == 0xFFFF && obj != 0) {
        D_004BD930[0] = D_00285780;
    }
}
