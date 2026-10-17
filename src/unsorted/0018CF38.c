/* UNIT 0x0018CF38 0x0018CF98 */
typedef struct Item { char p0[4]; unsigned char x4; } Item;
typedef struct Obj { char p0[0x514]; int x514; } Obj;
extern Item *D_003F4780[][14];
extern void func_0018E468(Obj *);
void func_0018CF38(Obj *o) {
 int i;
 for(i=0;i<14;i++) D_003F4780[o->x514][i]->x4=0;
 func_0018E468(o);
}
