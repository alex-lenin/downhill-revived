/* UNIT 0x0016CDE8 0x0016CE68 */
struct Obj { int x0; char pad4[0x6AC]; int x6B0,x6B4,x6B8; char x6BC,x6BD,x6BE,x6BF,x6C0; };
extern void func_0016EF78(void *);
extern void func_001224F0(void *,int,int,int,int);
void func_0016CDE8(struct Obj *p) {
 p->x0=0;
 func_0016EF78((char *)p+0x10);
 p->x6B0=0;
 func_001224F0((char *)p+0x250,0,0,-1,0);
 func_001224F0((char *)p+0x480,0,0,-1,0);
 p->x6B4=0; p->x6B8=0; p->x6BC=0; p->x6BD=0; p->x6BE=0; p->x6BF=0; p->x6C0=0;
}
