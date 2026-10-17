/* UNIT 0x0016CA70 0x0016CAB8 */
extern void func_0016D0C0(void *);
void func_0016CA70(char *obj) {
 char *part=obj+0x50;
 int count=9;
 do {
  --count;
  func_0016D0C0(part);
  part+=0x6D0;
 } while (count>=0);
}
