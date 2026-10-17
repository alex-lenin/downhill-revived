/* UNIT 0x00228040 0x00228078 */
extern char D_00720290[];
extern int D_00720270[];
void func_00228040(void **entry, int *value, int index) {
    *entry = (index * 0xde0) + D_00720290;
    *value = D_00720270[index];
}
