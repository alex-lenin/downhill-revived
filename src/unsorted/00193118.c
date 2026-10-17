/* UNIT 0x00193118 0x001931D0 */
extern int D_00768050[];
extern int D_0076F9E8[];
extern int func_001931D0(unsigned int index);
extern void func_00193208(unsigned int index);
extern int func_001932B8(unsigned int index, int item, int arg);

int func_00193118(unsigned int index, int arg) {
    int result = 0;
    int i;
    int count;
    int item;

    if (D_00768050[index] > 0) {
        count = D_0076F9E8[index];
        for (i = 0; i < count; i++) {
            item = func_001931D0(index);
            func_00193208(index);
            result = func_001932B8(index, item, arg);
            if (result != 0) {
                break;
            }
        }
    }
    return result;
}
