/* UNIT 0x0016DEC8 0x0016DF48 */
class Target {
public:
 virtual void v1();
 virtual void v2();
 virtual void v3();
 virtual unsigned long *v4();
};
struct Obj { char pad0[8]; int x8; char padC[0x124]; Target *x130; };
extern "C" void func_00126B20(Obj *,int);
extern "C" void func_0016DEC8(void *unused,Obj *p) {
 if(p->x8!=8) {
 unsigned long *flags=p->x130->v4();
 *flags &= 0xFDFFFFFFFFFFFFFFUL;
 func_00126B20(p,8);
 }
}
