/* UNIT 0x0016BC30 0x0016BC98 */
#include "scene.h"
typedef SceneNode Obj;
extern void func_0023AE58(int, int);
extern void func_0023AE78(int, int);
void func_0016BC30(Obj *obj, int id, int value, int arg) {
 if ((unsigned)(id-0x820)<0x191) {
  int index=obj->xB6;
  if(index>=0) {
   id += index*26; id -= 0x820;
   func_0023AE58(id,arg);
   func_0023AE78(id,value);
  }
 }
}
