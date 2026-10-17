/* UNIT 0x001A0958 0x001A09A8 */
typedef struct Item { int x0; char x4; } Item;
typedef struct Obj { char pad[0x510]; Item *x510, *x514; char x518; } Obj;
extern void func_00209060(Item *);
void func_001A0958(Obj *p) {
    p->x518 = 0;
    if (p->x510) p->x510->x4 = 0;
    if (p->x514) p->x514->x4 = 0;
    func_00209060(p->x510);
    func_00209060(p->x514);
}
