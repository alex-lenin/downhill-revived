/* UNIT 0x00115FA8 0x00116170 */
typedef struct Game { char pad0[0x5BB4]; int x5BB4; char pad5BB8[0x24]; int x5BDC; } Game;
typedef struct Rider { char pad0[0x796C]; int x796C; } Rider;
typedef struct Obj { char pad0[0x58]; Rider *x58; char pad5C[0x4C]; int xA8; } Obj;
extern Game D_00398C40;
void func_00115FA8(Obj *o) {
 int amount,rate;
 if(D_00398C40.x5BB4==0) amount=D_00398C40.x5BDC/2;
 else if(D_00398C40.x5BB4==2) amount=D_00398C40.x5BDC/2+50;
 else amount=D_00398C40.x5BDC/2+25;
 rate=amount*60/100;
 switch(o->x58->x796C) {
 case 0xE0: case 0x100: case 0x140: case 0x180: case 0x1A0:o->xA8=rate*8;break;
 case 0x40: o->xA8=rate*7;break;
 case 0x20: case 0x80: case 0xC0: o->xA8=rate*5;break;
 case 0x60: case 0x120:o->xA8=rate*2;break;
 case 0xA0: case 0x160:o->xA8=rate/2;break;
 case 0x1C0:o->xA8=0;break;
 case 0x1E0:default:o->xA8=0;break;
 }
 o->xA8+=60;
}
