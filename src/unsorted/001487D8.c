/* UNIT 0x001487D8 0x00148860 */
typedef struct Item { char pad0[0x4098]; unsigned long x4098; char pad1[0xE2C]; void *x4ECC; char pad2[0x1430]; char x6300[0x1684]; int x7984,x7988; } Item;
typedef struct Obj { char pad[0x2B4]; Item *x2B4; char gap[0xA4]; int x35C; } Obj;
extern void func_001486A8(Obj *);
void func_001487D8(Obj *p,Item *item) {
 Item *old=p->x2B4;
 if(old) {
 if(old->x4ECC==(char *)old+0x6320) return;
 if(old->x4ECC==(char *)old+0x62E0) return;
 if((unsigned char)(old->x4098>>2)&1) return;
 }
 if(item!=old) {
 if(old) { old->x7984=0; old->x7988=-1; }
 p->x2B4=item;
 }
 item=p->x2B4;
 if(item) { item->x7988=p->x35C; item->x7984=1; }
 func_001486A8(p);
}
