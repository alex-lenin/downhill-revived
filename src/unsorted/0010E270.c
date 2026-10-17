/* UNIT 0x0010E270 0x0010E420 */
typedef struct Detail { char p0[0x18]; float x18; } Detail;
typedef struct Owner { char p0[0x792C]; Detail *x792C; } Owner;
typedef struct Obj { Owner *x0; int x4; Owner *x8; char pC[0x294-0xC]; float x294; char p298[0x14]; float x2AC; char p2B0[0x158]; int x408; char p40C[0x3C]; unsigned long x448; } Obj;
extern int func_0010D650(Obj *);
extern int D_0039E7F4[];
void func_0010E270(Obj *obj) {
 float amount=0.0f;
 float speed=obj->x294;
 if(speed>200.0f) {
  if(speed>400.0f) {
   if(obj->x0->x792C->x18 > obj->x8->x792C->x18) amount=0.25f;
   else if(!(obj->x448 & 1)) amount=-0.1f;
  } else {
   amount=(speed-200.0f)*0.00125f;
   if(obj->x0->x792C->x18 < obj->x8->x792C->x18 && !(obj->x448 & 1)) amount=-amount;
  }
  if(!func_0010D650(obj)) amount=-0.1f;
 } else {
  unsigned long flags=obj->x448;
  if(flags & 4) amount=speed*0.000625f;
  else if(flags & 8) amount=-(speed*0.25f)*0.0025f;
 }
 obj->x2AC=amount+1.0f;
 if(obj->x2AC>1.0f && (!D_0039E7F4[0] || obj->x408>=61)) obj->x2AC=1.0f;
}
