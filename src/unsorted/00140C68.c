/* UNIT 0x00140C68 0x00140CE8 */
struct Obj { int x0,x4,x8,xC,x10,x14; char pad18[0x78]; char x90,x91; char pad92[2]; int x94,x98,x9C,xA0,xA4; char xA8[0x60]; char x108; char pad109[0x1B]; int x124,x128; char x12C; char pad12D[3]; int x130; };
void func_00140C68(struct Obj *p) {
 int count; char *q;
 p->x130=-1; p->x91=0; count=5; p->x90=0; q=p->xA8;
 p->x94=0;p->x98=0;p->x9C=0;p->xA0=0;
 p->x0=0;p->x4=0;p->x8=0;p->xC=0;p->x10=0;p->x14=0;
 p->xA4=0;p->x108=0;p->x124=0;p->x128=0;p->x12C=0;
 do {p->x124=0;count--;*q=0;q+=16;} while(count>=0);
}
