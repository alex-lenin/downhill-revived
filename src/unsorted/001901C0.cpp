/* UNIT 0x001901C0 0x00190320 */
class Target {
public:
 char pad0[0x4B9]; unsigned char x4B9; char pad4BA[0x3E];
 virtual void m1(); virtual void m2(); virtual void m3(); virtual void m4(); virtual void m5();
};
struct Obj { char pad0[4]; unsigned char x4; char pad5[0x127]; unsigned char x12C,x12D; char pad12E[0x22]; Target *x150; };
struct Global { char pad0[0x118]; int x118; char pad11C[0x16C]; int x288; char pad28C[0x264]; char x4F0[1]; };
extern Global D_004B25A0;
extern char D_004B2A90[];
extern int func_00139BF0();
extern int func_0018DD80(Target *,int,int);
extern void func_00190320(Obj *);
extern void func_00190358(Obj *);
extern void func_001B4DD0(Obj *);
extern void func_001B5130(Obj *);
extern void func_001B5A38(Obj *);
extern void func_001B5AC8(Obj *);
extern void func_001B5B70(Obj *,int);
extern void func_001C51C0(void *);
extern void func_001C54A0(void *);
void func_001901C0(Obj *obj) {
 Target *target;
 obj->x12D=0;
 obj->x12C=0;
 func_001B5A38(obj);
 func_001B5B70(obj,0);
 while(!obj->x12D) {
 func_001B4DD0(obj);
 target=obj->x150;
 if(!target->x4B9) { target->m1(); target->x4B9=1; }
 if(func_00139BF0()) func_001C51C0(D_004B2A90);
 else if(D_004B25A0.x118==2 && !obj->x4) func_001C54A0(D_004B25A0.x4F0);
 if(obj->x150) {
 if(!func_0018DD80(obj->x150,D_004B25A0.x288,1)) obj->x150->m5();
 else func_00190358(obj);
 }
 func_001B5130(obj);
 }
 func_001B5AC8(obj);
 func_00190320(obj);
}
