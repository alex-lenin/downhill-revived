/* UNIT 0x00115608 0x00115758 */
typedef struct Rider { char pad0[0x796C]; int x796C; } Rider;
typedef struct Obj { char pad0[0x58]; Rider *x58; char pad5C[0x10]; float x6C; char pad70[0x58]; int xC8; } Obj;
void func_00115608(Obj *obj) {
 float amount;
 int count;
 obj->x6C=0.0f; obj->xC8=0;
 switch(obj->x58->x796C) {
 case 0x40: amount=0.15f; count=5; break;
 case 0x100: case 0x180: case 0x1A0: case 0x140: case 0xC0: case 0xE0:
 amount=0.25f; count=11; break;
 case 0x80: case 0x60: amount=0.35f; count=14; break;
 case 0x120: case 0x20: amount=0.45f; count=17; break;
 case 0x1C0: amount=0.6f; count=20; break;
 case 0xA0: case 0x160: case 0x1E0: default: amount=0.6f; count=20; break;
 }
 obj->x6C=amount; obj->xC8=count;
}
