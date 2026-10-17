/* UNIT 0x00118FA8 0x00119040 */
typedef struct Item { char pad0[0xC]; short xC; } Item;
typedef struct Obj { char pad0[0x1004C]; int x1004C; } Obj;
typedef struct Global { char pad0[0x5BD8]; float x5BD8; } Global;
extern Global D_00398C40;
extern Item *func_001177F8(Obj *, int);
int func_00118FA8(Obj *obj) {
 int index = obj->x1004C - 2;
 for (; index > 0; index--) {
  int value = func_001177F8(obj, index)->xC;
  if (value >= 2) return (int)(value * D_00398C40.x5BD8);
 }
 return 0;
}
