/* UNIT 0x00176138 0x001761D8 */
typedef struct { float v[4]; } Vec;
typedef struct { char pad0[8]; unsigned short count; char padA[0x16]; int items[1]; } List;
typedef struct { char pad0[0xDE4]; Vec xDE4[1]; char padDF4[0x194]; List *xF88; } Obj;
extern void *func_00215B58(int);
extern void *func_0017C830(Obj *, void *);
extern void func_0017C8A8(Obj *, void *, Vec *);
void func_00176138(Obj *obj, List *list) {
 int i;
 obj->xF88 = list;
 for (i = 0; i < list->count; i++) {
  func_0017C8A8(obj, func_0017C830(obj, func_00215B58(list->items[i])), &obj->xDE4[i]);
 }
}
