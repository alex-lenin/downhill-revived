/* UNIT 0x001161F8 0x00116290 */
typedef struct Rider { char pad[0x796c]; int x796C; } Rider;
typedef struct Obj { char pad[0x58]; Rider *x58; char pad2[0xcc-0x5c]; int xCC; } Obj;
typedef struct World { char pad[0x12218]; short x12218[1]; } World;
extern World D_004225A0;
extern int D_004B26AC[];
void func_001161F8(Obj *obj) {
 Obj *saved=obj;
 int index=D_004B26AC[0];
 int selected=D_004225A0.x12218[index];
 int value;
 obj=(Obj *)obj->x58;
 value=((Rider *)obj)->x796C;
 if(value==selected) { saved->xCC|=0x800; }
 else {
 switch(value) {
 case 0xe0: case 0x100: case 0x140: case 0x180: case 0x1a0: saved->xCC|=0x800; break;
 default: saved->xCC&=~0x800; break;
 }
 }
}
