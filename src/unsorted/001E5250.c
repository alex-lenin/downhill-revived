/* UNIT 0x001E5250 0x001E5300 */
struct State { char pad[0x10c]; int x10c,x110; };
struct Obj { char p0[0x1c]; float x1c; char p20[0x580]; char x5a0[0x16c]; int x70c; char p710[0x9d8]; unsigned char x10e8,x10e9; };
extern struct State D_004B25A0;
extern char D_003985B0[],D_004C5640[];
extern void func_001692C8(void*,void*,int,int,int,int);
extern void func_001E4580(void*,void*,int,int,int,int);
void func_001E5250(struct Obj *obj) {
 if(D_004B25A0.x10c==5 && D_004B25A0.x110==1) {
  func_001692C8(D_003985B0,obj->x5a0,obj->x70c,(int)obj->x1c,obj->x10e8,obj->x10e9);
 } else if(D_004B25A0.x10c==11) {
  func_001E4580(D_004C5640,obj->x5a0,obj->x70c,(int)obj->x1c,obj->x10e8,obj->x10e9);
 }
}
