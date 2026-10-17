/* UNIT 0x0017CAA8 0x0017CB08 */
typedef struct Obj { char pad0[0x580]; int count; } Obj;
extern void func_0017CCB0(void *);
void func_0017CAA8(Obj *obj) {
 int index;
 for(index=0; index<obj->count; index++) {
  func_0017CCB0((char *)obj+index*0x2C);
 }
}
