/* UNIT 0x001BC618 0x001BC6A0 */
struct Entry { int x0; char pad[128]; };
struct Obj { char pad[0x644]; struct Entry entries[4]; };
extern int func_00235590(void *,int);
int func_001BC618(struct Obj *p,int key) {
 int i=0;
 do {
  if(p->entries[i].x0==key && func_00235590((char *)p+4,i)!=1) return i;
  i++;
 } while(i<4);
 return 5;
}
