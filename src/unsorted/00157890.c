/* UNIT 0x00157890 0x00157950 */
struct Child {int x0;char x4;};
struct Obj {char pad[0x110];int x110;unsigned int *x114[2];struct Child *x11C;};
extern int func_00139BF0(struct Obj *,unsigned int *);
extern struct Child *func_00208FB8();
extern void func_00218278(struct Child *,unsigned int *);
extern void func_00218498(unsigned int *,int,unsigned int **);
void func_00157890(struct Obj *p,unsigned int *data) {
 unsigned int **base,**slot;int offset;
 if(func_00139BF0(p,data)==0) {
 switch(*data>>18) {
 case 0x3EC:
 offset=p->x110*4;base=p->x114;*(unsigned int **)((char *)base+offset)=data;
 slot=base+p->x110;func_00218498(*slot,0,slot);p->x110++;break;
 case 0x3EE:
 if(!p->x11C)p->x11C=func_00208FB8();
 func_00218278(p->x11C,data);p->x11C->x4=0;break;
 }
 }
}
