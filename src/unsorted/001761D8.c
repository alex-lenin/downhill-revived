/* UNIT 0x001761D8 0x00176278 */
typedef struct { float v[4]; } Vec;
typedef struct { char pad0[8]; unsigned short count; char padA[0x16]; int items[1]; } List;
typedef struct { char pad0[0xD34]; Vec xD34[1]; } Obj;
extern void *func_00215B58(int);
extern void *func_0017C830(Obj *, void *);
extern void func_0017C8A8(Obj *, void *, Vec *);
void func_001761D8(Obj *obj, List *list) {
 int i;
 for (i = 0; i < list->count; i++) {
  func_0017C8A8(obj, func_0017C830(obj, func_00215B58(list->items[i])), &obj->xD34[i]);
 }
}
