/* UNIT 0x001A9640 0x001A9698 */
struct A { char pad0[0xc]; signed char *xC; char pad10[0x14]; int x24; char pad28[0x1c]; unsigned char x44; };
extern int func_00238F50(int, struct A *);
extern void func_00238BC8(int);
void func_001A9640(struct A *arg0) {
 int index = arg0->x24;
 signed char value = arg0->xC[index * 8 - 2];
 if (index > 0 && arg0->x44) {
  func_00238BC8(func_00238F50(index, arg0) + value);
 }
}
