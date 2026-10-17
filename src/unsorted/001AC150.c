/* UNIT 0x001AC150 0x001AC2E8 */
typedef struct Info { char pad[0x7908]; int x7908; char pad2[0x60]; int x796C; } Info;
typedef struct Obj { char pad[0x16D0]; Info *x16D0; } Obj;
extern void func_001B4000(Obj *,int);
extern void func_001AC140(Obj *,int,int);
extern void func_001AC328(Obj *,int);
void func_001AC150(Obj *obj) {
 func_001B4000(obj,1);
 switch(obj->x16D0->x796C) {
 case 0x20: func_001AC140(obj,0x3C,0x95); break;
 case 0x40: func_001AC140(obj,0x97,0x95); break;
 case 0x60: func_001AC140(obj,0x96,0x95); break;
 case 0x80: func_001AC140(obj,0x9B,0x95); break;
 case 0xA0: func_001AC140(obj,0x98,0x95); break;
 case 0xC0: func_001AC140(obj,0x99,0x95); break;
 case 0xE0: func_001AC140(obj,0x9E,0x95); break;
 case 0x100: func_001AC140(obj,0x9D,0x95); break;
 case 0x120: func_001AC140(obj,0x9A,0x95); break;
 case 0x140: func_001AC140(obj,0x9F,0x95); break;
 case 0x160: func_001AC140(obj,0x9C,0x95); break;
 case 0x180: func_001AC140(obj,0xA0,0x95); break;
 }
 if(obj->x16D0->x7908==31) func_001AC328(obj,1);
}
