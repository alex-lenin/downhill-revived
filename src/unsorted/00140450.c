/* UNIT 0x00140450 0x001404B8 */
typedef struct { int x0,x4,x8[2],x10,x14[11],x40; } Obj;
void func_00140450(Obj *o) {
 int i;
 o->x4=0; o->x10=0; o->x0=0;
 for(i=1;i>=0;i--) o->x8[i]=0;
 for(i=10;i>=0;i--) o->x14[i]=0;
 o->x40=0;
}
