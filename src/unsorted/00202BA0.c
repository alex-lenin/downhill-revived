/* UNIT 0x00202BA0 0x00202BD0 */
void func_00202BA0(unsigned short *values, int x, int y, int amount) {
    values += (x * x + x) / 2 + y;
    *values += amount;
}
