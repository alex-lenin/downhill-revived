/* UNIT 0x00196138 0x00196198 */
typedef struct Entry { char pad[20]; } Entry;
typedef struct Obj { char p0[24]; Entry entries[152]; char pBF8[8]; int xC00; } Obj;
extern void func_00196888(Entry *);
void func_00196138(Obj *o) {
 int i;
 for(i=0;i<o->xC00;i++) func_00196888(&o->entries[i]);
}
