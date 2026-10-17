/* UNIT 0x0021A4E8 0x0021A530 */
typedef struct { short x0; unsigned char x2,x3; unsigned int x4; } Entry;
int func_0021A4E8(Entry *entries, unsigned char first, unsigned char second, short *index) {
 int i=0;
 do {
  if(entries->x2==first && entries->x3==second) { *index=i; return 1; }
  i++; entries++;
 } while(i<15);
 return 0;
}
