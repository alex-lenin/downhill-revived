/* UNIT 0x00135990 0x001359F8 */
typedef struct Name { int x0,x4; } Name;
typedef struct Child { char pad[0x7A69]; unsigned char x7A69; char pad7A6A[0x7BA4-0x7A6A]; int x7BA4; } Child;
typedef struct Obj { char pad[0x158]; Child *x158; char pad15C[0x4B4-0x15C]; Name x4B4; } Obj;
extern Name D_0029C390[], D_0029C3A0[];
extern void func_00135A30(Obj *);
void func_00135990(Obj *obj) {
 obj->x158->x7BA4=0;
 if(obj->x158->x7A69) obj->x4B4=D_0029C3A0[0];
 else obj->x4B4=D_0029C390[0];
 func_00135A30(obj);
}
