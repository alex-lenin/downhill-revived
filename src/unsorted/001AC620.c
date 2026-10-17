/* UNIT 0x001AC620 0x001AC680 */
typedef struct Vector { float x, y, z, w; } Vector;
typedef struct Obj { Vector *x0; Vector x4; } Obj;
extern void func_00203380(Vector *, Vector *, float);
void func_001AC620(Obj *obj, Vector *src) {
    Vector copy = *src;
    func_00203380(&copy, &obj->x4, 0.0f);
    obj->x0 = src;
}
