/* UNIT 0x00196460 0x001964A8 */
extern void func_00196820(void *);
void func_00196460(char *obj) {
 char *p = obj + 0x18;
 int count = 149;
 do { count--; func_00196820(p); p += 20; } while (count >= 0);
}
