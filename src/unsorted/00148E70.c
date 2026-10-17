/* UNIT 0x00148E70 0x00148F10 */
typedef struct Obj { char pad0[0x2B0]; int x2B0, x2B4; char pad2B8[0x354-0x2B8]; int x354; char pad358[0x10]; int x368; } Obj;
extern void func_00148860(Obj *);
extern int func_00238F60(void);
extern void func_0014CC38(Obj *);
extern int func_0023D608(void);
extern void func_0014D6A8(Obj *);
extern void func_00148F10(Obj *);
extern void func_00221ED0(Obj *, void *);
void func_00148E70(Obj *obj) {
 if (obj->x2B0 && obj->x2B4) {
  if (obj->x354) func_00148860(obj);
  obj->x368 += func_00238F60();
  func_0014CC38(obj);
  if (func_0023D608() >= 2) func_0014D6A8(obj);
  func_00148F10(obj);
  func_00221ED0(obj, (char *)obj+0x40);
  obj->x354=0;
 }
}
