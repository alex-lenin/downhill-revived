/* UNIT 0x001DCCD8 0x001DCD38 */
typedef struct Item { char pad[0x10]; float x10,x14,x18; int x1C; } Item;
typedef struct Obj { char pad[0x30]; Item *x30,*x34; } Obj;
extern void func_00211280(Item *);
void func_001DCCD8(Obj *p) {
 if(p->x30) {
 p->x30->x10=p->x34->x10; p->x30->x14=p->x34->x14; p->x30->x18=p->x34->x18; p->x30->x1C=0; func_00211280(p->x30);
 }
}
