/* UNIT 0x00208AB0 0x00208AE0 */
struct Obj { int items[20]; int x50; };
struct Obj *func_00208AB0(struct Obj *obj) {
    int count=19; int *p=&obj->items[19];
    do { *p=0; count--; p--; } while(count>=0);
    obj->x50=0; return obj;
}
