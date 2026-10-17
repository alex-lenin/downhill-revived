/* UNIT 0x00222B08 0x00222B68 */
extern char D_00663140[];
extern int func_00243198(void *);
int func_00222B08(int low, int high) {
    int result;
    if (low < high) result = low + ((unsigned int)((high-low+1)*(func_00243198(D_00663140)&0x7FFF)) >> 15);
    else result = low;
    return result;
}
