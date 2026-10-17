/* UNIT 0x0016D5C0 0x0016D648 */
typedef struct { int x0,x4,x8; } Part;
typedef struct { char pad0[0x250]; Part x250; char pad25C[0x224]; Part x480; char pad48C[0x234]; unsigned char x6C0; } Obj;
void func_0016D5C0(Obj *obj) {
 if (!obj->x6C0) {
  Part *part=&obj->x250;
  if (part->x0 || part->x4) goto set_first;
  if (part->x8 >= 6) goto second;
  if (part->x8 < 4) goto second;
set_first:
  obj->x6C0=1;
second:
  part=&obj->x480;
  if (part->x0 || part->x4) goto set_second;
  if (part->x8 >= 6) return;
  if (part->x8 < 4) return;
set_second:
  obj->x6C0=1;
 }
}
