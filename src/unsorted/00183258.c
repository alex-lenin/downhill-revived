/* UNIT 0x00183258 0x001832B0 */
typedef struct { int x0[4],x10[4],x20,x24,x28,x2c,x30; } Obj;
extern struct { char p[0x288]; int count; } D_004B25A0;
void func_00183258(Obj *o) {
int i; int *p=(int *)o;
o->x24=0; o->x2c=1; o->x20=1; o->x28=0; o->x30=0;
for(i=0;i<D_004B25A0.count;i++){p[0]=0;p[4]=0;p++;}
}
