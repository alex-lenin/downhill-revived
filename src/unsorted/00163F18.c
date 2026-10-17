/* UNIT 0x00163F18 0x00163F98 */
struct Item { float x0,x4,x8; char pad[20]; };
struct Obj { struct Item *items; };
struct List { char pad[0x16]; short count; short indices[1]; };
struct Item *func_00163F18(struct Obj *obj,struct List *list) {
 int i,best; float value,current; int count; short *p;
 i=0;value=-999999.9f;best=0;
 if (list->count>0) { p=list->indices; do {
  current=obj->items[*p].x8;
  if(value<current) {value=current;best=i;}
  i++;p++;
 } while(i<list->count); }
 return &obj->items[list->indices[best]];
}
