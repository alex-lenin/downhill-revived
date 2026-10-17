/* UNIT 0x001726F8 0x00172758 */
typedef struct Obj { char pad0[0x888]; int count; } Obj;
extern void func_00172A00(void *);
void func_001726F8(Obj *obj) {
 char *item;
 int index;
 for(index=0; index<obj->count; index++) {
  func_00172A00((char *)obj+index*0x130);
 }
}
