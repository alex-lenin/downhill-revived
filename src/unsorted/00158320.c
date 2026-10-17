/* UNIT 0x00158320 0x00158370 */
struct Obj {
    int x0, x4, x8;
    char xC, xD, xE, xF, x10, x11, x12, x13;
};
extern void func_00158370(struct Obj *);
void func_00158320(struct Obj *obj, int value) {
    obj->x0 = value;
    obj->x12 = 2;
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    obj->xD = 0;
    obj->xE = 0;
    obj->xF = 0;
    obj->x11 = 1;
    obj->x10 = 0;
    obj->x13 = 1;
    func_00158370(obj);
}



