/* UNIT 0x001B1100 0x001B1150 */
struct Item { char pad[0x110]; };
struct Obj { struct Item items[4]; int x440; };
extern void func_001B13E0(struct Item *);
void func_001B1100(struct Obj *o,int n) {
 struct Item *p; struct Item *end;
 o->x440=n;
 p=(struct Item *)o; end=p+4;
 do { func_001B13E0(p); p++; } while((int)p<(int)end);
}
