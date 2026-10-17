/* UNIT 0x001930C8 0x00193118 */
extern int D_00768050[];
extern int func_00193118(unsigned int, int);
int func_001930C8(unsigned int index, int arg1) {
    int result = 0;
    if (index < 17U) {
        if (D_00768050[index] > 0) result = func_00193118(index, arg1);
    }
    return result;
}
