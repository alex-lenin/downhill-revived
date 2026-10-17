/* UNIT 0x00112B20 0x00112CD0 */
typedef struct Pos { char pad[0x18]; float x18; } Pos;
typedef struct Rider {
 char pad[0x4098];
 unsigned long flags;
 char pad2[0x792c - 0x40a0];
 Pos *x792c;
 char pad3[0x7a68 - 0x7930];
 unsigned char x7a68;
} Rider;
typedef struct Obj {
 Rider *x0;
 int x4;
 Rider *x8;
 char pad[0x408 - 0xc];
 int x408, x40c;
 char pad2[0x438 - 0x410];
 int x438;
 char pad3[0x448 - 0x43c];
 long flags;
} Obj;
extern float func_001385A8(void *);
void func_00112B20(Obj *o) {
 long flags; int old; float delta, diff, threshold, speed;
 flags = o->flags;
 o->flags = flags & ~0x10;
 old = (int)(flags >> 4) & 1;
 if (o->x8 != 0 && o->x8->x7a68 == 0) {
  if (!((unsigned char)(o->x0->flags >> 2) & 1) && o->x40c < 1500) {
   delta = o->x8->x792c->x18 - o->x0->x792c->x18;
   speed = func_001385A8((char *)o->x0 + 0x40c0);
   diff = speed - func_001385A8((char *)o->x8 + 0x40c0);
   speed = 0.07f;
   if (diff > speed) delta += (diff - speed) * 250.0f;
   if (delta > -10.0f && o->x408 >= 121) {
    threshold = 0.3f;
    if (old) threshold = 0.03f;
    if (threshold < func_001385A8((char *)o->x0 + 0x40c0)) {
     speed = func_001385A8((char *)o->x0 + 0x40c0);
     if (func_001385A8((char *)o->x8 + 0x40c0) * 0.8f < speed) {
      o->x438 = 0;
      o->flags |= 0x10;
     }
    }
   }
  }
 }
}
