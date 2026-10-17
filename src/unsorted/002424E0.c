/* UNIT 0x002424E0 0x00242540 */
typedef struct Obj { int x0; char pad[0x40]; int x44,x48,x4C,x50; } Obj;
extern void func_00202AD0(int, int, int, int, int);
int func_002424E0(Obj *obj) {
 func_00202AD0(obj->x44, (obj->x48 / 1024) * 1024, obj->x44 + obj->x50, 0, 0);
 obj->x0 = 2; return 1;
}
