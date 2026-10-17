/* UNIT 0x0017BA78 0x0017BB18 */
typedef struct List { char pad[8]; unsigned short count; char padA[0x16]; int items[1]; } List;
typedef struct Obj { char pad[0xF04]; int values[10]; int items[10]; char pad54[0x14]; List *xF68; } Obj;
extern int func_00215B58(int);
extern int func_0017C830(Obj *, int);
void func_0017BA78(Obj *obj, List *list) {
 int item; int i;
 obj->xF68 = list;
 for (i=0; i<list->count; i++) {
  item=list->items[i];
  obj->values[i]=func_0017C830(obj,func_00215B58(item));
  obj->items[i]=item;
 }
}
