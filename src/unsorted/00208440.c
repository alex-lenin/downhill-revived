/* UNIT 0x00208440 0x002084A0 */
typedef struct { int *x0; int x4, x8, xC; } Entry;
typedef struct { Entry entries[12]; int xC0; } Obj;
void func_00208440(Obj *obj, int *value, int a, int b) {
    if (*value != 0) {
        obj->entries[obj->xC0].x0 = value;
        obj->entries[obj->xC0].x4 = a;
        obj->entries[obj->xC0].x8 = b;
        obj->entries[obj->xC0].xC = 0;
        obj->xC0++;
    }
}
