/* UNIT 0x00169728 0x00169770 */
int func_00169728(char *obj, int *ids) {
 int result=1;
 if (*ids != 15) {
  int flags=*(int *)(obj+0x53C);
  do {
   int mask=1 << *ids++;
   result = (mask & flags) ? result : 0;
  } while (*ids != 15);
 }
 return result;
}
