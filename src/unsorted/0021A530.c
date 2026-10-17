/* UNIT 0x0021A530 0x0021A578 */
typedef struct { short x0; unsigned char x2,x3; unsigned int x4; } Entry;
unsigned int func_0021A530(Entry *entries, unsigned int limit, short *index) {
 unsigned int best=0;
 int i=0;
 if(!limit) limit=0x100000;
 do {
  if(entries->x4<limit && best<entries->x4) {
   *index=i; best=entries->x4;
  }
  i++; entries++;
 } while(i<15);
 return best;
}
