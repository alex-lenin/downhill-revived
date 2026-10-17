/* UNIT 0x00144248 0x001442D0 */
typedef struct Item { unsigned long flags; char pad[0x38]; char x40[1]; } Item;
typedef struct Obj { Item *x0; int x4; char gap[8]; char x10[0x70]; char x80[1]; } Obj;
extern char D_0036CFC0[];
extern void func_00221F20(void *);
extern void func_001DCDC8(void *);
extern void func_001DDB58(void *,void *);
extern void func_001442D0(Obj *,int);
void func_00144248(Obj *p,int arg,Item *item,int unused,int last) {
 p->x0=item;
 if(item) { item->flags |= (unsigned long)0x8000 << 42; func_00221F20(p->x0->x40); }
 p->x4=arg;
 func_001DCDC8(p->x10);
 func_001DDB58(p->x80,D_0036CFC0);
 func_001442D0(p,last);
}
