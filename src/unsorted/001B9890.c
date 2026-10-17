/* UNIT 0x001B9890 0x001B98F0 */
struct Obj { char pad[0x10c]; int x10C, x110; char pad1[0x140]; int x254[15]; int x290[15]; };
extern char D_004225A0[];
void func_001B9890(struct Obj *obj, int index, int value) {
 obj->x254[index]=value;
 if(obj->x110==1) {
  char *entry = D_004225A0 + ((obj->x290[index]>>5)-1)*0x694;
  (obj->x10C*0x50 + entry)[0x30]=value;
 }
}
