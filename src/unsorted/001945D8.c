/* UNIT 0x001945D8 0x00194648 .rodata=0x28D8F0:0x28D920 */
extern int func_00194C80(void *, int);
int func_001945D8(char *arg0, int arg1, int arg2) {
    int value = func_00194C80(arg0 + arg2 * 0x694, arg1);
    int result = 0;
    switch (arg1) {
    case 0: case 1: case 2: result = value >= 4; break;
    case 3: result = value >= 19; break;
    case 4: case 5: case 6: case 7: case 8: case 9: default: break;
    }
    return result;
}
