/* UNIT 0x001183E0 0x00118458 */
typedef struct Obj { char pad0[0x10894]; int x10894, x10898; char pad1089C[3]; unsigned char x1089F; } Obj;
extern int func_00118F10(Obj *);
extern int func_00118FA8(Obj *);
void func_001183E0(Obj *obj) {
 int value;
 obj->x10894 = func_00118F10(obj);
 value = func_00118FA8(obj);
 obj->x10898 = value;
 if (obj->x10894 >= 2) {
  if (value >= 2) {
   obj->x1089F = 1;
   return;
  }
 }
 obj->x1089F = 0;
}
