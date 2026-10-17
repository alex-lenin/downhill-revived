/* UNIT 0x001404B8 0x00140538 */
struct Obj { unsigned int *x0, *x4, *x8, *xC, *x10; unsigned int *x14[11]; int x40; };
void func_001404B8(struct Obj *obj, unsigned int *data) {
 switch (*data >> 18) {
 case 0x1E: obj->x8 = data; return;
 case 0x1F: obj->xC = data; return;
 case 0x385: obj->x10 = data; return;
 case 0x386: obj->x4 = data; return;
 case 0x389: obj->x14[obj->x40] = data; return;
 }
}
