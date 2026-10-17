/* UNIT 0x001C3638 0x001C3668 */
int func_001C3638(char *p) {
 int *q = (int *)(p+0x20); int i = 0;
loop:
 if(*q) { i++; q++; if(i>=5) return -1; goto loop; }
 return i;
}
