/* UNIT 0x0013C6A8 0x0013C828 */
typedef struct Entry { char x0[6]; unsigned char x6; char x7[9]; } Entry;
typedef struct VEntry { short delta; short pad; void (*fn)(void *); } VEntry;
struct Child { int x0; char pad4[0x14]; Entry x18[64]; int x418; int x41C; char pad420[0xD8]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); };
typedef struct Obj { char pad0[0x45C]; int x45C; char pad460[0xA0]; Child *x500; char pad504[0x1C]; int x520; } Obj;
extern void func_0013C400(Obj *, int);
extern void func_0018E5A8(Child *);
extern int func_00190D60(void *, int, int);
extern void func_00203590(void *, int, int, int);
extern float func_00203C28(void *);
struct SubManager { int func_00190D60(int, int); };
extern struct Manager { char pad[100]; SubManager x64; } D_003F52D0;
void func_0013C6A8(Obj *obj) {
 int tmp[3];
 int selected, reset, ready, i;
 Child *child;
 Entry *entry;
 SubManager *manager;
 ready = 1;
 if (obj->x520 >= 0) {
  tmp[2] = 0;
  func_00203590(tmp, 0x2588, 0, obj->x520);
  if (func_00203C28(tmp) < 0.9f)
   ready = 0;
 }
 Manager *g = &D_003F52D0;
 child = obj->x500;
 manager = &g->x64;
 reset = 0;
 selected = child->x418;
 i = selected - 1;
 if (i >= 0) {
  int off = i * 16 + 0x18;
  entry = (Entry *)(off + (int)child);
  do {
   if (entry->x6)
    selected = i;
   i--;
   entry--;
  } while (i >= 0);
 }
 if (func_00190D60(manager, 0, obj->x45C)) {
  Child *other = obj->x500;
  if (other->x41C == 50 && (other->x0 == selected || other->x418 == 1)) {
   func_0013C400(obj, 0);
   Child *updated = obj->x500;
   if (updated)
    updated->x0 = -1;
   reset = 1;
  }
 }
 Child *post = obj->x500;
 if (post && ready) {
  if (reset) {
   post->x0 = -1;
   func_0018E5A8(obj->x500);
   post = obj->x500;
   post->v5();
  } else {
   post->v4();
  }
 }
}
