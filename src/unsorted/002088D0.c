/* UNIT 0x002088D0 0x00208900 */
struct Obj { int items[30]; int x78; };
struct Obj *func_002088D0(struct Obj *obj) {
    int count=29; int *p=&obj->items[29];
    do { *p=0; count--; p--; } while(count>=0);
    obj->x78=0; return obj;
}
