/* UNIT 0x0019E950 0x0019E9A0 */
struct Obj { char p0[0x10]; int x10; int p14; int x18; char p1c[0x30]; int x4c,x50; char p54[0x12c]; int x180; char p184[12]; int x190; char p194[0x94]; int x228; unsigned char x22c; char p22d[0x257]; int x484; };
void func_0019E950(struct Obj *o) {
 int i; int *p;
 o->x484=0; o->x10=0; o->x18=0; o->x4c=0; o->x180=0; o->x190=0; o->x50=0; o->x22c=0;
 p=&o->x228;
 for(i=34;i>=0;i--){ *p=0; p--; }
}
