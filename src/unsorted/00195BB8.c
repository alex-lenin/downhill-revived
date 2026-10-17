/* UNIT 0x00195BB8 0x00195C40 */
struct Obj { int x0,x4,x8; char xC[4],x10[4],x14,x15,x16,x17[16],x27; };
void func_00195BB8(struct Obj *p) {
 int i; char *q;
 p->x4=0x3C3; p->x8=0x200; p->x0=0x399; p->x27=0;
 q=p->x10; i=3;
 do { q[-4]=0; i--; q[0]=0; q++; } while(i>=0);
 p->x15=0; p->x14=1; p->x16=0;
 q=(char *)p+0x26; i=15;
 do { *q=0; i--; q--; } while(i>=0);
}
