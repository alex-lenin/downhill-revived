/* UNIT 0x001B5B40 0x001B5B70 */
void func_001B5B40(unsigned char *p) {
 int count = 8; unsigned char *q = p+8;
 do { *q = 0; count--; q--; } while(count >= 0);
 p[0x61] = 0;
}
