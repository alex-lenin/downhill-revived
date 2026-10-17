/* UNIT 0x0015C118 0x0015C1F8 */
struct Child { char *x0; virtual void a(); virtual void b(); virtual int test(); };
extern void func_0015BFF8(void *, void *);
void func_0015C118(Child *obj) {
 char *base = obj->x0;
 Child *child;
 if ((unsigned int)(*(int *)(base + 0x7908) - 35) < 3) {
  Child *dest = (Child *)(base + 0x63DC);
  if (!obj->test()) {
   child = (Child *)(base + 0x6400);
   if (child->test()) goto found;
   child = (Child *)(base + 0x63E8);
   if (child->test()) {
found:
    func_0015BFF8(dest, child);
   } else {
    Child *last = (Child *)(base + 0x63F4);
    if (last->test()) func_0015BFF8(dest, last);
   }
  }
 }
}
