/* UNIT 0x001AC6F8 0x001AC778 */
struct Range { int x0; float min,max; };
struct Table { char pad[0x10]; struct Range *ranges[1]; };
struct Owner { char pad[0x20]; struct Table *table; };
struct Link { struct Owner *owner; char pad[8]; short index; };
struct Obj { struct Link *link; char pad[0x30]; int x34; float x38,x3c; };
extern int func_00202DA8(void);
void func_001AC6F8(struct Obj *o, float t) {
 struct Link *link=o->link;
 if(link) {
  struct Owner *owner=link->owner;
  if(owner) {
   struct Range *r=owner->table->ranges[link->index];
   float min=r->min;
   float f=(r->max-min)*t+min;
   o->x34=func_00202DA8();
   o->x38=f;
   o->x3c=1.0f;
  }
 }
}
