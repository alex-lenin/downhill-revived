/* UNIT 0x00144C18 0x00144C78 */
typedef struct Vector { int x0, x4, x8; } Vector;
typedef struct Obj {
    int x0, x4, x8, xC, x10, x14;
    char pad18[8];
    int x20, x24;
    char pad28[8];
    int x30, x34;
    char pad38[0x148];
    Vector x180;
    int x18C, x190, x194, x198, x19C, x1A0, x1A4, x1A8, x1AC, x1B0, x1B4;
} Obj;
static inline void clear_vector(Vector *v) {
    v->x0 = 0;
    v->x4 = 0;
    v->x8 = 0;
}
void func_00144C18(Obj *obj) {
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    obj->x10 = 0;
    obj->x14 = 0;
    obj->x20 = 0;
    obj->x24 = 0;
    obj->x30 = 0;
    obj->x34 = 0;
    clear_vector(&obj->x180);
    obj->x190 = 0;
    obj->x194 = 0;
    obj->x198 = 0;
    obj->x19C = 0;
    obj->x1A0 = 0;
    obj->x1A4 = 0;
    obj->x1A8 = 0;
    obj->x1AC = 0;
    obj->x1B0 = 0;
    obj->x1B4 = 0;
}
