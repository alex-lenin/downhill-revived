/* UNIT 0x00169770 0x001697F0 */
struct Obj { char pad0[0x500]; int x500[8]; int x520; char pad524[0x18]; int x53C; short x540; };
extern int func_001697F0(struct Obj *,int);
void func_00169770(struct Obj *p,int index) {
 if(func_001697F0(p,index)==0) {
 p->x500[p->x520]=index;
 if(p->x540<=0) p->x540=180;
 p->x520++;
 p->x53C |= 1<<index;
 }
}
