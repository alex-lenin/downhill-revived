/* UNIT 0x00148B68 0x00148BA8 */
struct Obj { char pad[0x2B4]; void *x2B4; char pad2[0xB48]; int xE00; };
int func_00148B68(struct Obj *obj, void *target) {
 int i; struct Obj *entry=obj;
 if(target) { for(i=0; i<obj->xE00; i++,entry=(struct Obj *)((char *)entry+0x380)) {
 if(target==entry->x2B4) return i;
 } }
 return -1;
}
