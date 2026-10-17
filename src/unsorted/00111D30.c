/* UNIT 0x00111D30 0x00111D98 */
typedef struct Pos { char pad0[0x18]; float x18; } Pos;
typedef struct Owner { char pad0[0x792C]; Pos *x792C; char pad7930[0x70]; int x79A0; } Owner;
typedef struct Obj { Owner *x0; char pad4[0x78]; int x7C, x80; char pad84[0x384]; int x408; char pad40C[8]; int x414; } Obj;
typedef struct Data { char pad0[0x5019C]; float x5019C; } Data;
extern Data D_002AF2C0;
int func_00111D30(Obj *obj) {
 obj->x7C = 0;
 obj->x80 = 0;
 obj->x414 = 0;
 if (obj->x408 < 0) {
  Owner *owner = obj->x0;
  if (owner->x792C->x18 - D_002AF2C0.x5019C > 75.0f && owner->x79A0 == 0) owner->x79A0 = 1;
 }
 return 1;
}
