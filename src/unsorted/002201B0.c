/* UNIT 0x002201B0 0x00220218 */
extern unsigned char D_0029E2A8[8];
extern int func_00220230(int index);
extern int func_002204A8(int);

int func_002201B0(int index) {
    int result = 0;
    if (D_0029E2A8[index]) {
        if (func_00220230(index) || func_002204A8(index)) result = 1;
    }
    return result;
}
