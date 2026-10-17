/* UNIT 0x0019E9A0 0x0019E9E8 */
void func_0019E9A0(char *obj) {
 int count = 9;
 int *p = (int *)(obj + 0x1C);
 char *b = obj + 0x62;
 do {
  b[-10] = 0;
  count--;
  b[0] = 0;
  *p = 0;
  b++; p++;
 } while (count >= 0);
 *(int *)(obj + 0x44) = 0;
 *(int *)(obj + 0x48) = 0;
 *(int *)(obj + 0x488) = 0;
 *(int *)(obj + 0) = 0;
 *(int *)(obj + 4) = 0;
 obj[0x6C] = 0;
}
