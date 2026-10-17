/* UNIT 0x001449F8 0x00144A40 */
extern void func_00144E50(void *);
void func_001449F8(char *p) {
 int count=11;
 p+=0x10;
 do { count--; func_00144E50(p); p+=0x1C0; } while(count>=0);
}
