/* UNIT 0x0018D718 0x0018D7B0 */
struct Entry { int x0,x4,x8; unsigned char xC; char pad[7]; };
extern struct Entry D_003F4BA0[];
extern void func_0018D6F0(struct Entry *);
struct Entry *func_0018D718(int a,int b,int c) {
 struct Entry *entry=D_003F4BA0; int active=1;
 do {
  if(entry->xC) ++entry;
  else {
   func_0018D6F0(entry);
   entry->xC=active; entry->x0=a; entry->x4=b; entry->x8=c;
   return entry;
  }
 } while((int)entry<(int)(D_003F4BA0+20));
 return 0;
}
