/* UNIT 0x00119700 0x00119790 */
struct Node { char pad[0x14]; unsigned short x14; short pad16; unsigned short x18; };
extern struct Node *func_001177F8(void *, short);
void func_00119700(void *obj, short *index, unsigned short *out) {
 int count=0; struct Node *node;
 while(count<7) {
  node=func_001177F8(obj,*index);
  *out=node->x18;
  if ((node->x14 >> 10)&1) break;
  count++;
  *index=node->x18;
 }
}
