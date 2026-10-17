/* UNIT 0x00222BD8 0x00222C38 */
extern int func_002559D8(void);
int func_00222BD8(int low, int high) {
    int result;
    if (low < high) result = low + (((high-low+1)*(func_002559D8()&0x7FFF)) >> 15);
    else result = low;
    return result;
}
