/* UNIT 0x001E2FB0 0x001E3060 */
typedef struct Obj {
 char pad0[0x500];
 int cur;
 int x504[4];
 char pad514[0xC];
 unsigned char names[4][13];
 unsigned char count[4];
} Obj;

void func_001E2FB0(Obj *obj, int pos) {
 int i;
 if (obj->count[obj->cur] != 0) {
  obj->x504[obj->cur] = 1;
  for (i = pos; i < 12; i++) {
   obj->names[obj->cur][i] = obj->names[obj->cur][i + 1];
  }
  obj->names[obj->cur][12] = '!';
  obj->count[obj->cur]--;
 }
}
